namespace Duckvil
{
    public struct Quaternion
    {
        public float x { get; set; }
        public float y { get; set; }
        public float z { get; set; }
        public float w { get; set; }

        public Quaternion()
        {
            this.x = 0;
            this.y = 0;
            this.z = 0;
            this.w = 1;
        }

        public Quaternion(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        // Vector2 XY => new Vector2(x, y);
        // Vector2 YZ => new Vector2(y, z);
        // Vector2 XZ => new Vector2(x, z);
    };

}