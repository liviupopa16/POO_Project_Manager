#include "Manager_Window.h"

Manager_Window::Manager_Window(QWidget *parent) : QWidget(parent)
{
	ui = new Ui::Manager_Window();
	ui->setupUi(this);
}

Manager_Window::~Manager_Window()
{
	delete ui;
}
