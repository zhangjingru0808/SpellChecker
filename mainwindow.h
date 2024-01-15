#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>

const int MATCH = 0;       
const int MISMATCH = 1;    
const int VC_MISMATCH = 3; 
const int GAP = 2;         

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

public slots:
    void checkSlot();
    void loadDictFile();

private:
	Ui::MainWindow* ui;
	std::map<std::string, int> dict;

	int sequence_alignment(const std::string& s1, const std::string& s2);
	int penalty(char x, char y);
	bool is_vowel(char c);
};
#endif 
