using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Collections.Specialized;
using System.IO;
using System.Diagnostics;

class IniHandler
{
	// ini File path
	public string FileName; //INI文件名
	//声明读写INI文件的API函数
	[DllImport("kernel32")]
	private static extern bool WritePrivateProfileString(string section, string key, string val, string filePath);
	[DllImport("kernel32")]
	private static extern int GetPrivateProfileString(string section, string key, string def, byte[] retVal, int size, string filePath);
	//类的构造函数，传递INI文件名
	public IniHandler(string AFileName)
	{
		// 判断文件是否存在
		FileInfo fileInfo = new FileInfo(AFileName);
		//Todo:搞清枚举的用法
		if ((!fileInfo.Exists))
		{ //|| (FileAttributes.Directory in fileInfo.Attributes))
			//文件不存在，建立文件
			System.IO.StreamWriter sw = new System.IO.StreamWriter(AFileName, false, System.Text.Encoding.Default);
			try
			{
				sw.Write("");
				sw.Close();
			}
			catch
			{
				throw (new ApplicationException("Ini文件不存在"));
			}
		}
		//必须是完全路径，不能是相对路径
		FileName = fileInfo.FullName;
	}
	//确保资源的释放
	~IniHandler()
	{
		updateFile();
	}

	//写INI文件
	public void writeString(string Section, string Ident, string Value)
	{
		if (!WritePrivateProfileString(Section, Ident, Value, FileName))
		{
			throw (new ApplicationException("写Ini文件出错"));
		}
	}
	//读取INI文件指定
	public string readString(string Section, string Ident, string Default)
	{
		Byte[] Buffer = new Byte[65535];
		int bufLen = GetPrivateProfileString(Section, Ident, Default, Buffer, Buffer.GetUpperBound(0), FileName);
		//必须设定0（系统默认的代码页）的编码方式，否则无法支持中文
		string s = Encoding.GetEncoding(0).GetString(Buffer);
		s = s.Substring(0, bufLen);
		return s.Trim();
	}
	//读整数
	public int readInteger(string Section, string Ident, int Default)
	{
		string intStr = readString(Section, Ident, Convert.ToString(Default));
		try
		{
			return Convert.ToInt32(intStr);
		}
		catch (Exception ex)
		{
			Debug.WriteLine(string.Format("Exception: ReadInteger. {0}", ex.Message));
			return Default;
		}
	}
	//写整数
	public void writeInteger(string Section, string Ident, int Value)
	{
		writeString(Section, Ident, Value.ToString());
	}
	//读布尔
	public bool readBool(string Section, string Ident, bool Default)
	{
		try
		{
			return Convert.ToBoolean(readString(Section, Ident, Convert.ToString(Default)));
		}
		catch (Exception ex)
		{
			Debug.WriteLine(string.Format("Exception: ReadBool. {0}", ex.Message));
			return Default;
		}
	}
	//写Bool
	public void writeBool(string Section, string Ident, bool Value)
	{
		writeString(Section, Ident, Convert.ToString(Value));
	}
	//从Ini文件中，将指定的Section名称中的所有Ident添加到列表中
	public void readSection(string Section, StringCollection Idents)
	{
		Byte[] Buffer = new Byte[16384];
		//Idents.Clear();
		int bufLen = GetPrivateProfileString(Section, null, null, Buffer, Buffer.GetUpperBound(0),
		  FileName);
		//对Section进行解析
		getStringsFromBuffer(Buffer, bufLen, Idents);
	}
	private void getStringsFromBuffer(Byte[] Buffer, int bufLen, StringCollection Strings)
	{
		Strings.Clear();
		if (bufLen != 0)
		{
			int start = 0;
			for (int i = 0; i < bufLen; i++)
			{
				if ((Buffer[i] == 0) && ((i - start) > 0))
				{
					String s = Encoding.GetEncoding(0).GetString(Buffer, start, i - start);
					Strings.Add(s);
					start = i + 1;
				}
			}
		}
	}
	//从Ini文件中，读取所有的Sections的名称
	public void readSections(StringCollection SectionList)
	{
		//Note:必须得用Bytes来实现，StringBuilder只能取到第一个Section
		byte[] Buffer = new byte[65535];
		int bufLen = 0;
		bufLen = GetPrivateProfileString(null, null, null, Buffer,
										Buffer.GetUpperBound(0), FileName);
		getStringsFromBuffer(Buffer, bufLen, SectionList);
	}
	//读取指定的Section的所有Value到列表中
	public void readSectionValues(string Section, NameValueCollection Values)
	{
		StringCollection KeyList = new StringCollection();
		readSection(Section, KeyList);
		Values.Clear();
		foreach (string key in KeyList)
		{
			Values.Add(key, readString(Section, key, ""));
		}
	}
	/**/////读取指定的Section的所有Value到列表中，
	//public void readSectionValues(string Section, NameValueCollection Values,char splitString)
	//{   string sectionValue;
	//    string[] sectionValueSplit;
	//    StringCollection KeyList = new StringCollection();
	//    readSection(Section, KeyList);
	//    Values.Clear();
	//    foreach (string key in KeyList)
	//    {
	//        sectionValue=readString(Section, key, "");
	//        sectionValueSplit=sectionValue.Split(splitString);
	//        Values.Add(key, sectionValueSplit[0].ToString(),sectionValueSplit[1].ToString());

	//    }
	//}
	//清除某个Section
	public void eraseSection(string Section)
	{
		//
		if (!WritePrivateProfileString(Section, null, null, FileName))
		{
			throw (new ApplicationException("无法清除Ini文件中的Section"));
		}
	}
	//删除某个Section下的键
	public void deleteKey(string Section, string Ident)
	{
		WritePrivateProfileString(Section, Ident, null, FileName);
	}
	//Note:对于Win9X，来说需要实现UpdateFile方法将缓冲中的数据写入文件
	//在Win NT, 2000和XP上，都是直接写文件，没有缓冲，所以，无须实现UpdateFile
	//执行完对Ini文件的修改之后，应该调用本方法更新缓冲区。
	public void updateFile()
	{
		WritePrivateProfileString(null, null, null, FileName);
	}
	//检查某个Section下的某个键值是否存在
	public bool valueExists(string Section, string Ident)
	{
		//
		StringCollection Idents = new StringCollection();
		readSection(Section, Idents);
		return Idents.IndexOf(Ident) > -1;
	}

	// 以FileStream方式，在写ini文件指写Section的注释
	public void writeSectionComment(string Section, string Comment)
	{
		//防止文本字符中有特殊字符。必须用Encoding.Default
		StreamReader reader = new StreamReader(FileName, Encoding.Default);

		String a = reader.ReadToEnd();
		reader.Close();

		int index = a.IndexOf(Section);
		if (index != -1)
		{
			a = a.Insert(index, replaceBadCharOfFileName(Comment + "\r\n"));

			FileStream stream = new FileStream(FileName, FileMode.OpenOrCreate, FileAccess.Write);
			stream.Seek(0, SeekOrigin.Begin);
			stream.SetLength(0);
			StreamWriter writer = new StreamWriter(stream, Encoding.Default);
			writer.Write(a);
			writer.Close();
			stream.Close();
		}
		else
		{
			Trace.WriteLine(string.Format("Warning: No section [{0}], in DrvCfg.ini File.", Section));
		}
	}

	// 删除"[]"开头的空section
	public void deleteEmptySection()
	{
		// 防止文本字符中有特殊字符。必须用Encoding.Default
		StreamReader reader = new StreamReader(FileName, Encoding.Default);

		String a = reader.ReadToEnd();
		// Section替换为Comment + Section
		string tmp = @"[]" + "\r\n";
		a = a.Replace(tmp, "");
		// FileName重命名为tmp.txt
		// 防止文本字符中有特殊字符。必须用Encoding.Default
		StreamWriter readTxt = new StreamWriter(@"tmp.txt", false, Encoding.Default);
		readTxt.Write(a);
		readTxt.Flush();
		readTxt.Close();
		reader.Close();
		//b.hhp重命名为a.hhp,并删除b.hhp
		File.Copy(@"tmp.txt", FileName, true);
		File.Delete(@"tmp.txt");
	}
	/// <summary>
	/// 去掉文件名中的无效字符,如 \ / : * ? " < > | 
	/// </summary>
	/// <param name="fileName">待处理的文件名</param>
	/// <returns>处理后的文件名</returns>
	static public string replaceBadCharOfFileName(string fileName)
	{
		string str = fileName;
		str = str.Replace("\\", string.Empty);
		str = str.Replace("/", string.Empty);
		str = str.Replace(":", string.Empty);
		str = str.Replace("*", string.Empty);
		str = str.Replace("?", string.Empty);
		str = str.Replace("\"", string.Empty);
		str = str.Replace("\0", string.Empty);
		str = str.Replace("<", string.Empty);
		str = str.Replace(">", string.Empty);
		str = str.Replace("|", string.Empty);
		// str = str.Replace(" ", string.Empty);    //前面的替换会产生空格,最后将其一并替换掉
		return str;
	}
}
