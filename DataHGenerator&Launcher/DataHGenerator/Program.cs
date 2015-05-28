using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;

namespace DataHGenerator
{
    class Program
    {
        static byte[] _buffer = new byte[1024];
        static int _count;

        static void Main(string[] args)
        {
            if (args.Length != 2)
            {
                PrintHelp();
                return;
            }

            string folder = args[0];
            string header = args[1];

            if (!Path.IsPathRooted(folder))
                folder = Path.Combine(Directory.GetCurrentDirectory(), folder);

            if (!Path.IsPathRooted(header))
                header = Path.Combine(Directory.GetCurrentDirectory(), header);

            if (!Directory.Exists(folder))
            {
                Console.WriteLine("Invalid path: \"" + folder + "\"");
                return;
            }

            File.WriteAllText(Path.Combine(Path.GetDirectoryName(header), "DataBlock.h"), "#ifndef DATA_BLOCK_H\n#define DATA_BLOCK_H\nstruct DataBlock { int Size; char* Data; };\n#endif;");

            string headerName = Path.GetFileNameWithoutExtension(header).ToUpper() + "_H";
            using (FileStream fileOut = new FileStream(header, FileMode.Create))
            {
                using (StreamWriter writer = new StreamWriter(fileOut, Encoding.UTF8))
                {
                    writer.WriteLine("#ifndef " + headerName);
                    writer.WriteLine("#define " + headerName);
                    writer.WriteLine();
                    writer.WriteLine("#include <DataBlock.h>");
                    writer.WriteLine();

                    foreach (string file in Directory.GetFiles(folder))
                    {
                        using (FileStream fileIn = new FileStream(file, FileMode.Open))
                        {
                            string name = Path.GetFileName(file).Replace('.', '_');
                            string dname = name + "_DATA";
                            string lname = name + "_SIZE";
                            writer.WriteLine("#define " + lname + " " + fileIn.Length.ToString());
                            writer.Write("char " + dname + "[] = {");

                            while ((_count = fileIn.Read(_buffer, 0, _buffer.Length)) > 0)
                            {
                                for (int x = 0; x < _count; x++)
                                {
                                    writer.Write("(char)0x" + _buffer[x].ToString("X2"));
                                    if (fileIn.Position - _count + x < fileIn.Length - 1)
                                        writer.Write(", ");
                                }
                            }
                            writer.Write("};" + Environment.NewLine);

                            writer.WriteLine("DataBlock " + name + " = {" + lname + ", " + dname + "};"); 
                        }
                    }

                    writer.WriteLine();
                    writer.WriteLine("#endif");
                }
            }

            Console.WriteLine("Done!");
        }

        static void PrintHelp()
        {
            Console.WriteLine(Path.GetFileName(Assembly.GetEntryAssembly().Location) + " [Data folder] [Header file name]");
        }
    }
}
