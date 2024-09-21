using System;
using System.Configuration;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace modbucTCPinterop
{
    public partial class Form1 : Form
    {
        private wrapperModbusTCP.connectionWrapper connection;
        private wrapperModbusTCP.DataQueueWrapper dataQueue;
        private wrapperModbusTCP.DataRetrievalWrapper dataRetrieval;

        public Form1()
        {
            InitializeComponent();
        }

        private static DateTime Delay(int MS)
        {
            DateTime ThisMoment = DateTime.Now;
            TimeSpan duration = new TimeSpan(0, 0, 0, 0, MS);
            DateTime AfterWards = ThisMoment.Add(duration);

            while (AfterWards >= ThisMoment)
            {
                System.Windows.Forms.Application.DoEvents();
                ThisMoment = DateTime.Now;
            }

            return DateTime.Now;
        }

        private void socketConnectionButton_Click(object sender, EventArgs e)
        {
            try
            {
                string ipAddress = ConfigurationManager.AppSettings["IpAddress"];
                int port = int.Parse(ConfigurationManager.AppSettings["Port"]);

                connection = new wrapperModbusTCP.connectionWrapper(ipAddress, port);
                connection.connectWrapper();

                int queueSize = int.Parse(ConfigurationManager.AppSettings["QueueSize"]);
                dataQueue = new wrapperModbusTCP.DataQueueWrapper();

                MessageBox.Show("Connected successfully!");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to connect: {ex.Message}");
            }
        }

        private void socketDisconnectionButton_Click(object sender, EventArgs e)
        {
            try
            {
                connection.disconectWrapper(); // 메서드 이름이 정확히 맞는지 확인해주세요
                MessageBox.Show("Disconnected successfully!");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to disconnect: {ex.Message}");
            }
        }

        private void retrievalStartButton_Click(object sender, EventArgs e)
        {
            try
            {
                byte functionCode = byte.Parse(ConfigurationManager.AppSettings["FunctionCode"]);
                ushort startAddress = ushort.Parse(ConfigurationManager.AppSettings["StartAddress"]);
                ushort numRegisters = ushort.Parse(ConfigurationManager.AppSettings["NumRegisters"]);

                dataRetrieval = new wrapperModbusTCP.DataRetrievalWrapper(connection, dataQueue);

                dataRetrieval.setFunctionCodeWrapper(functionCode);
                dataRetrieval.setStartAddressWrapper(startAddress);
                dataRetrieval.setNumRegistersWrapper(numRegisters);

                dataRetrieval.startWrapper();

                Task.Run(() => RetrieveDataPeriodically());

                MessageBox.Show("Started data retrieval successfully!");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to start data retrieval: {ex.Message}");
            }
        }

        private void retrievalStopButton_Click(object sender, EventArgs e)
        {
            try
            {
                if (dataRetrieval != null)
                {
                    dataRetrieval.stopWrapper();
                    MessageBox.Show("Stopped data retrieval successfully!");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Failed to stop data retrieval: {ex.Message}");
            }
        }

        private unsafe void RetrieveDataPeriodically()
        {
            while (true)
            {
                if (dataRetrieval != null && !dataQueue.isEmptyWrapper())
                {
                    int length = 0;
                    byte* dataPtr = dataQueue.popWrapper(ref length); // dataQueue 사용
                    if (dataPtr != null)
                    {
                        byte[] data = new byte[length];
                        for (int i = 0; i < length; i++)
                        {
                            data[i] = dataPtr[i];
                        }

                        StringBuilder hex = new StringBuilder(data.Length * 2);
                        foreach (byte b in data)
                        {
                            hex.AppendFormat("{0:x2} ", b);
                        }

                        Invoke(new Action(() => {
                            richTextBox1.AppendText($"Hex data: {hex.ToString()}\n");
                        }));
                    }
                }
                else
                {
                    Invoke(new Action(() => richTextBox1.AppendText("Queue is empty\n")));
                }

                Delay(510); // 0.5초 간격으로 데이터 수집
            }
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            if (dataRetrieval != null)
            {
                dataRetrieval.stopWrapper();
                dataRetrieval = null;
            }

            if (connection != null)
            {
                connection.disconectWrapper(); // 메서드 이름이 정확히 맞는지 확인해주세요
                connection = null;
            }

            base.OnFormClosing(e);
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
    }
}


