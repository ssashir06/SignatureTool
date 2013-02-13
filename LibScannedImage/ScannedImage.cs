using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;
using System.Drawing;

namespace CameraScanner
{
	public interface ScannedImage
	{
		void SaveImage(String path, String file_name);
		String ToString();

		[XmlIgnore]
		Image Page { get; set; }
		String FileName { get; }
		DateTime TimeTaken { get; set; }
	}
}
