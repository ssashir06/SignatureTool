using System;
using System.Drawing;
using System.IO;

namespace Signature.CountingTool.Database {
    public partial class SignatureCounterDataSet
    {
		public partial class ImageFileRow
        {
			public String FullPath
            {
                get
                {
                    return Path.Combine(Directory, FileName);
                }
            }
        }
        public partial class TrimRow
        {
            public override string ToString()
            {
                return String.Format("({0},{1})-({2},{3})", x, y, x + w, y + h);
            }

			public static explicit operator Rectangle(TrimRow src)
            {
                return new Rectangle((int)src.x, (int)src.y, (int)src.w, (int)src.h);
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
