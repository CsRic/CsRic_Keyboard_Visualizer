using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Windows.Forms.DataVisualization.Charting;

namespace CsRic_Keyboard_Visualizer
{
    public unsafe partial class Form1 : Form
    {
        [DllImport(@"CoreFunction.dll", EntryPoint = "FreqAnalyze", CallingConvention = CallingConvention.Cdecl)]
        extern static int FreqAnalyze(int* nHistograms, float** value);
    
        ~Form1()
        {

        }
        public Form1()
        {
            InitializeComponent();
            //allocate static memory
            nHistograms = (int*)Marshal.AllocHGlobal(sizeof(int));
            value = (float**)Marshal.AllocHGlobal(sizeof(IntPtr));
            *nHistograms = 99999;
            //multi threads
            thread1 = new Thread(new ThreadStart(CalculateValue));
            thread1.Start();
            Thread.Sleep(100);

            //initialize chart
            SetChart1();

            //new array
            
            valueSlower = new float[*nHistograms];
            valueSlower.Initialize();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            FormClosed += new System.Windows.Forms.FormClosedEventHandler(Form1_Close);
        }

        private void chart1_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            Visualize();
        }

        int* nHistograms;
        float** value;
        Series series;
        float[] valueSlower;
        Thread thread1;

        private void CalculateValue()
        {
            FreqAnalyze(nHistograms, value);
        }

        private void Visualize()
        {
            const float fallSpeed = 0.80f;
            for (int i = *nHistograms - 1; i >= 0; i--)
            {
                double temp = (*value)[i];
                if (temp > 100) temp = 100;
                valueSlower[i] = (float)(temp > valueSlower[i] ? temp : valueSlower[i] * fallSpeed > 3 ? valueSlower[i] * fallSpeed : 0.0f);
                double[] temp2 = { valueSlower[i] };

                series.Points[i].YValues = temp2;
                textBox1.Text = (*nHistograms).ToString();
            }
        }
        private void SetChart1()
        {
            //initialize chart
            while (*nHistograms > 200)
            {
                Thread.Sleep(100);
            }
            series = chart1.Series[0];
            series.ChartType = SeriesChartType.Column;
            series.BorderWidth = 8;
            series.Color = System.Drawing.Color.Red;
            int x = 0;
            for (int i = 0; i < *nHistograms; i++)
            {
                series.Points.AddXY(x, 20);
            }
            ChartArea chartArea = chart1.ChartAreas[0];
            chartArea.AxisX.Minimum = 0;
            chartArea.AxisX.Maximum = *nHistograms + 1;
            chartArea.AxisX.LineColor = System.Drawing.Color.White;
            chartArea.AxisY.Minimum = 0d;
            chartArea.AxisY.Maximum = 100d;
            chartArea.AxisY.Enabled = AxisEnabled.False;
            chartArea.BackColor = System.Drawing.Color.Black;
            
        }
        private void Form1_Close(object sender, EventArgs e)
        {
            timer1.Enabled = false;
            thread1.Abort();
            Marshal.FreeHGlobal((IntPtr)nHistograms);
            Marshal.FreeHGlobal((IntPtr)value);
            System.Environment.Exit(0);
        }
    }
}
