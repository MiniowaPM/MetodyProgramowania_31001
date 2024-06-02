#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>
#include "Globals.h"

wxIMPLEMENT_APP(App);

std::vector<Matrix> Matrices;

Matrix* selectedMatrix;

bool App::OnInit() {
	MainFrame* mainFrame = new MainFrame("Matrix Calc");
	mainFrame->SetClientSize(800,600);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}