using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using Microsoft.CodeAnalysis.Text;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Xml;

namespace Duckvil.SourceGenerator
{
    public static class SyntaxNodeExtensions
    {
        public static T GetParent<T>(this SyntaxNode node)
        {
            var parent = node.Parent;

            while (parent != null)
            {
                if (parent is T t)
                {
                    return t;
                }

                parent = parent.Parent;
            }

            throw new Exception();
        }
    }

    public class CppSharedSyntaxReceiver : ISyntaxReceiver
    {
        public record Capture(ClassDeclarationSyntax klass, List<MethodDeclarationSyntax> methods, List<FieldDeclarationSyntax> variables, List<PropertyDeclarationSyntax> properties);

        public List<Capture> captures { get; } = new();

        public void OnVisitSyntaxNode(SyntaxNode node)
        {
            if (node is not ClassDeclarationSyntax cds)
            {
                return;
            }

            var methods = cds.Members
                .Select(m => m as MethodDeclarationSyntax)
                .Where(m => m != null)
                .Where(
                    m => m.AttributeLists.Any(
                        a => a.Attributes.Any(
                            x => x.Name.ToString() == "CppShared"
                        )
                    )
                )
                .ToList();

            var variables = cds.Members
                .Select(m => m as FieldDeclarationSyntax)
                .Where(m => m != null)
                .Where(
                    m => m.AttributeLists.Any(
                        a => a.Attributes.Any(
                            x => x.Name.ToString() == "CppShared"
                        )
                    ) && m.Declaration.Variables.Any(x => x.Identifier.Text.Contains("_an_"))
                )
                .ToList();

            var properties = cds.Members
                .Select(m => m as PropertyDeclarationSyntax)
                .Where(m => m != null)
                .Where(
                    m => m.AttributeLists.Any(
                        a => a.Attributes.Any(
                            x => x.Name.ToString() == "CppShared"
                        )
                    ) && m.Identifier.Text.Contains("_an_")
                )
                .ToList();

            captures.Add(new Capture(cds, methods, variables, properties));
        }
    }

    [Generator]
    public class CppSharedGenerator : ISourceGenerator
    {
        public void Initialize(GeneratorInitializationContext context)
        {
/*#if DEBUG
            if (!Debugger.IsAttached)
            {
                Debugger.Launch();
            }
#endif*/

            context.RegisterForSyntaxNotifications(() => new CppSharedSyntaxReceiver());
        }

        public void Execute(GeneratorExecutionContext context)
        {
            var r = (CppSharedSyntaxReceiver)context.SyntaxReceiver;

            string _a = "using System.Runtime.CompilerServices;\n";

            var curr = r.captures.First().klass.GetParent<NamespaceDeclarationSyntax>();
            uint count = 0;

            while (true)
            {
                try
                {
                    _a += "namespace " + curr.Name.ToString() + " {\n";

                    count++;

                    curr = curr.GetParent<NamespaceDeclarationSyntax>();
                }
                catch (Exception)
                {
                    break;
                }
            }

            _a += $@"public partial class {r.captures.First().klass.Identifier.Text} : Duckvil.SharedScript
{{
    private ulong sharedScriptID;

    public {r.captures.First().klass.Identifier.Text}()
    {{
        Duckvil.SharedScript.Register(out sharedScriptID, this);

        Console.WriteLine(""Constructor {{0}} {{1}}"", ID, GetSharedScriptID());
    }}

    public ulong GetSharedScriptID()
    {{
        return sharedScriptID;
    }}

    public void SetSharedScriptID(ulong id)
    {{
        sharedScriptID = id;
    }}

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void VariableChanged(ulong sharedScriptID, string name, object value);
";

            foreach (var cppShared in r.captures)
            {
                foreach (var variable in cppShared.variables)
                {
                    var name = variable.Declaration.Variables.First().Identifier.Text.Replace("_an_", "");

                    _a += $@"
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void VariableChanged_{name}(ulong sharedScriptID, string name, object value);

    [Duckvil.CppShared]
    public {variable.Declaration.Type.ToString()} {name}
    {{ 
        get
        {{
            return {variable.Declaration.Variables.First().Identifier.Text};
        }}
        set
        {{
            {variable.Declaration.Variables.First().Identifier.Text} = value;

            VariableChanged_{name}(GetSharedScriptID(), ""{variable.Declaration.Variables.First().Identifier.Text.Replace("_an_", "")}"", {variable.Declaration.Variables.First().Identifier.Text});
        }}
    }}
";
                }

                foreach (var property in cppShared.properties)
                {
                    _a += $@"
    public {property.Type.ToString()} {property.Identifier.Text.Replace("_an_", "")}
    {{ 
        get
        {{
            return {property.Identifier.Text};
        }}
        set
        {{
            {property.Identifier.Text} = value;

            VariableChanged(GetSharedScriptID(), ""{property.Identifier.Text.Replace("_an_", "")}"", {property.Identifier.Text});
        }}
    }}
";
                }

                foreach (var meth in cppShared.methods)
                {
                    string p = "";

                    if (meth.ParameterList.Parameters.Count > 0)
                    {
                        p += ", ";
                        p += meth.ParameterList.ToString().Replace("(", "").Replace(")", "");
                    }

                    string p2 = "";

                    if (meth.ParameterList.Parameters.Count > 0)
                    {
                        p2 += ", ";
                        p2 += string.Join(",", meth.ParameterList.Parameters.Select(x => x.Identifier.Text).ToList());
                    }

                    _a += $@"
    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    private extern static void {meth.Identifier.Text}(ulong sharedScriptID{p});

    partial {meth.ReturnType.ToString()} {meth.Identifier.Text}{meth.ParameterList.ToString()}
    {{
        {meth.Identifier.Text}(GetSharedScriptID(){p2});
    }}
";
                }

                _a += @$"
}}
";
            }

            for (; count> 0; count--)
            {
                _a += "}";
            }

            context.AddSource($"{r.captures.First().klass.Identifier.Text}.g.cs", _a);
        }
    }
}
