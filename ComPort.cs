using System;
using System.Collections.Generic;
using System.Linq;
using System.IO.Ports;
using System.Collections;
//using System.Windows.Forms;
using System.Text;
using System.Threading.Tasks;

namespace Microsoft.Samples.Kinect.BodyBasics
{
    class ComPort
    {
        public static Queue ByteList = Queue.Synchronized(new Queue());
        private static SerialPort aSerialPor = null;
        public static int FsByteSum = 0;
        private static string aErrMsg = "";
        public static int JsByteSum = 0;
        //public static System.Windows.Forms.Timer timer_Ret;
        //    public static byte[] ByteList2 = {0x01, 0x02, 0x03 };
        //     public static byte[] ByteList3 = { MainWindow.xDistance};

        public static void InitComPort()
        {
            aSerialPor = new SerialPort();
            aSerialPor.BaudRate = 115200;
            aSerialPor.DataBits = 8;
            aSerialPor.StopBits = StopBits.One;
            aSerialPor.Parity = Parity.None;
            aSerialPor.PortName = "COM5";
           // timer_Ret = new System.Windows.Forms.Timer();
            OpenCom();
            //timer_Ret.Interval = 1000;
            //timer_Ret.Tick += new System.EventHandler(timer_Ret_Tick);
            //timer_Ret.Start();
        }

        public static bool OpenCom()
        {
            if (aSerialPor == null)
            {
                aErrMsg = "未初始化的COM对象！";
                return false;
            }
            //set recieved event
            try
            {
                aSerialPor.DataReceived += new SerialDataReceivedEventHandler(aSerialPor_DataReceived);

                aSerialPor.Open();
                JsByteSum = 0;
            }
            catch (Exception Err)
            {
                aErrMsg = Err.Message;
                return false;
            }
            return true;
        }

        public static bool ComSetData(byte[] SetData)
        {
            if (aSerialPor == null)
            {
                aErrMsg = "未初始化COM对象";
                return false;
            }
            if (aSerialPor.IsOpen == false)
            {
                aErrMsg = "未打开的COM对象";
                return false;
            }

            aSerialPor.DiscardOutBuffer();

            try
            {
                    aSerialPor.Write(SetData, 0, SetData.Length);

                    FsByteSum += SetData.Length;
            }
            catch (Exception Err)
            {
                aErrMsg = Err.Message;
                return false;
            }
            return true;
        }

        private static void aSerialPor_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int iByteLen = aSerialPor.BytesToRead;
            byte[] iBytes;
            iBytes = new byte[iByteLen];
            aSerialPor.Read(iBytes, 0, iByteLen);
            JsByteSum += iByteLen;
            ByteList.Enqueue(iBytes);
        }

        private static void timer_Ret_Tick(object sender, EventArgs e)
        {
            // ComSetData(ComPort.ByteList3);
        }
    }
}
