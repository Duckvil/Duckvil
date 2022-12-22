namespace Duckvil
{
    public struct Vector2
    {
        public float x { get; set; }
        public float y { get; set; }

        public Vector2()
        {
            this.x = 0;
            this.y = 0;
        }

        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }
    };

}