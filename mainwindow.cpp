#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent), ui(new Ui::MainWindow) {
	ui->setupUi(this);
	setWindowTitle("SpellChecker");
	connect(ui->checkbuttn, &QPushButton::clicked, this, &MainWindow::checkSlot);
    loadDictFile();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::checkSlot() {
	QString word = ui->lineinput->text();
	if (word.isEmpty()) {
		QMessageBox::warning(this, "ERROR", "please input a word");
		return;
	}


	for (auto& w : dict)
	{
		w.second = 0;
		int sa = sequence_alignment(word.toStdString(), w.first);
		w.second = sa;
	}

	std::vector< std::pair<std::string, int> > vec;
	for (std::map<std::string, int>::iterator it = dict.begin(); it != dict.end(); it++) {
		vec.push_back(std::pair<std::string, int>(it->first, it->second));
	}
    std::sort(vec.begin(), vec.end(), [](std::pair<std::string, int>a, std::pair<std::string, int>b) {return a.second <= b.second; });

	ui->checkoutput->clear();
	int index = 0;
	for (auto d : vec)
	{
		if (d.second > 0 && index <= 10)
		{
			ui->checkoutput->appendPlainText(QString::fromStdString(d.first));
			index++;
		}

        if(d.second==4)
        {
            qDebug()<< QString::fromStdString( d.first);
        }
	}
}
void MainWindow::loadDictFile() {
    QString strPath("dictionary.txt");

    QFile f(strPath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "ERROR", "can not open file");
        ui->statusbar->showMessage("open dict file failed");
        return;
    }
    dict.clear();
    QTextStream txtInput(&f);
    QString word;
    while (!txtInput.atEnd())
    {
        word = txtInput.readLine();
        dict.insert(std::pair<std::string, int>(word.toStdString(), 0));
    }
    f.close();

}

int MainWindow::sequence_alignment(const std::string& s1, const std::string& s2) {
	int m = s1.length();
	int n = s2.length();

	std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));

	for (int i = 0; i <= m; i++) {
		dp[i][0] = i * GAP;
	}
	for (int j = 0; j <= n; j++) {
		dp[0][j] = j * GAP;
	}

	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			dp[i][j] = std::min(
				{ dp[i - 1][j - 1] +
					 penalty(s1[i - 1], s2[j - 1]),
				 dp[i - 1][j] + GAP,               
				 dp[i][j - 1] + GAP });             
		}
	}
	return dp[m][n];
}

int MainWindow::penalty(char x, char y) {
	if (x == y) {
		return MATCH;
	}
	else if (is_vowel(x) && is_vowel(y)) {
		return MISMATCH;
	}
	else if (!is_vowel(x) && !is_vowel(y)) {
		return MISMATCH;
	}
	else {
		return VC_MISMATCH;
	}
}
bool MainWindow::is_vowel(char c) {
	c = toupper(c); 
	return (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}
