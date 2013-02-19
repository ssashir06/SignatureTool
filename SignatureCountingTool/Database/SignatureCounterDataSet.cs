using System;

namespace SignatureCountingTool.Database {
    public partial class SignatureCounterDataSet
    {
        public partial class TrimRow
        {
            public override string ToString()
            {
                return String.Format("({0},{1})-({2},{3})", x, y, x + w, y + h);
            }
        }
		public partial class TypeRow
        {
			public override String ToString()
            {
                return String.Format("{0}: {1}", ID, Name);
            }
        }
    }
}
