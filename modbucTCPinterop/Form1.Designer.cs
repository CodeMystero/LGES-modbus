namespace modbucTCPinterop
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다. 
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마세요.
        /// </summary>
        private void InitializeComponent()
        {
            this.socketConnectionButton = new DevExpress.XtraEditors.SimpleButton();
            this.socketDisconnectionButton = new DevExpress.XtraEditors.SimpleButton();
            this.retrievalStartButton = new DevExpress.XtraEditors.SimpleButton();
            this.retrievalStopButton = new DevExpress.XtraEditors.SimpleButton();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // socketConnectionButton
            // 
            this.socketConnectionButton.Location = new System.Drawing.Point(18, 17);
            this.socketConnectionButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.socketConnectionButton.Name = "socketConnectionButton";
            this.socketConnectionButton.Size = new System.Drawing.Size(97, 23);
            this.socketConnectionButton.TabIndex = 0;
            this.socketConnectionButton.Text = "소켓연결";
            this.socketConnectionButton.Click += new System.EventHandler(this.socketConnectionButton_Click);
            // 
            // socketDisconnectionButton
            // 
            this.socketDisconnectionButton.Location = new System.Drawing.Point(18, 43);
            this.socketDisconnectionButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.socketDisconnectionButton.Name = "socketDisconnectionButton";
            this.socketDisconnectionButton.Size = new System.Drawing.Size(97, 21);
            this.socketDisconnectionButton.TabIndex = 1;
            this.socketDisconnectionButton.Text = "소켓해제";
            this.socketDisconnectionButton.Click += new System.EventHandler(this.socketDisconnectionButton_Click);
            // 
            // retrievalStartButton
            // 
            this.retrievalStartButton.Location = new System.Drawing.Point(18, 68);
            this.retrievalStartButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.retrievalStartButton.Name = "retrievalStartButton";
            this.retrievalStartButton.Size = new System.Drawing.Size(97, 21);
            this.retrievalStartButton.TabIndex = 2;
            this.retrievalStartButton.Text = "검색시작";
            this.retrievalStartButton.Click += new System.EventHandler(this.retrievalStartButton_Click);
            // 
            // retrievalStopButton
            // 
            this.retrievalStopButton.Location = new System.Drawing.Point(18, 93);
            this.retrievalStopButton.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.retrievalStopButton.Name = "retrievalStopButton";
            this.retrievalStopButton.Size = new System.Drawing.Size(97, 21);
            this.retrievalStopButton.TabIndex = 3;
            this.retrievalStopButton.Text = "검색중지";
            this.retrievalStopButton.Click += new System.EventHandler(this.retrievalStopButton_Click);
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(134, 17);
            this.richTextBox1.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(1204, 344);
            this.richTextBox1.TabIndex = 4;
            this.richTextBox1.Text = "";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1347, 378);
            this.Controls.Add(this.richTextBox1);
            this.Controls.Add(this.retrievalStopButton);
            this.Controls.Add(this.retrievalStartButton);
            this.Controls.Add(this.socketDisconnectionButton);
            this.Controls.Add(this.socketConnectionButton);
            this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private DevExpress.XtraEditors.SimpleButton socketConnectionButton;
        private DevExpress.XtraEditors.SimpleButton socketDisconnectionButton;
        private DevExpress.XtraEditors.SimpleButton retrievalStartButton;
        private DevExpress.XtraEditors.SimpleButton retrievalStopButton;
        private System.Windows.Forms.RichTextBox richTextBox1;
    }
}

