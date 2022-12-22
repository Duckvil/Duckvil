using System.Runtime.CompilerServices;
using Duckvil;

namespace Sandbox
{
    public partial class TestSharedScript : Entity
    {
        private TransformComponent _transform;

        // public TestSharedScript()
        // {
        //     Console.WriteLine("Constructor! {0}", ID);

        //     test(2);
        // }

        [CppShared]
        public int _an_a;

        [CppShared]
        partial void HelloFrom(int a);

        public void Initialize()
        {
            // transform = GetComponent<TransformComponent>();

            // Vector3 _t = transform.Translation;

            // transform.Translation = _t;

            // AddComponent(new TransformComponent());

            TestSharedScript test = new TestSharedScript();

            test.HelloFrom(2);
            test.HelloFrom(4);

            AddComponent<TransformComponent>();
            _transform = GetComponent<TransformComponent>() ?? throw new Exception();

            Console.WriteLine("Something! {0} {1}", ID, GetSharedScriptID());

            HelloFrom(1);
            HelloFrom(3);

            a++;
        }

        public void Update(double delta)
        {
            var t = _transform.Translation;

            t += new Vector3((float)delta * a, 0, 0);

            _transform.Translation = t;
        }

        public void Say(int a)
        {
            Console.WriteLine("Saying: " + a.ToString());
        }
    }

}