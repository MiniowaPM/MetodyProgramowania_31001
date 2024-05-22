#include "MainFrame.h"
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/gdicmn.h> // wxSize
#include <wx/image.h> // imageHandler

enum ButtonIDs {
	BUTTON_ID_ADD = 2,
	BUTTON_ID_DELETE = 3, 
	BUTTON_ID_COPY = 4,
	BUTTON_ID_EDIT = 5,
	BUTTON_ID_MATFUNC = 6,
	BUTTON_ID_DROP = 7
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_BUTTON(BUTTON_ID_ADD, MainFrame::OnButtonClickedAdd)
	EVT_BUTTON(BUTTON_ID_DELETE, MainFrame::OnButtonClickedDelete)
	EVT_BUTTON(BUTTON_ID_COPY, MainFrame::OnButtonClickedCopy)
	EVT_BUTTON(BUTTON_ID_EDIT, MainFrame::OnButtonClickedEdit)
	EVT_BUTTON(BUTTON_ID_MATFUNC, MainFrame::OnButtonClickedMatFunc)
	EVT_BUTTON(BUTTON_ID_DROP, MainFrame::OnButtonClickedDrop)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {

	// Panel layout and styling section
	wxPanel* toolBar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200,100));
	toolBar->SetBackgroundColour(wxColor(100, 100, 200));

	wxPanel* listBar = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(300, 100));
	listBar->SetBackgroundColour(wxColor(200, 100, 100));

	wxPanel* mainScreen = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	mainScreen->SetBackgroundColour(wxColor(100, 200, 100));


	// Creating basic sizers layout
	wxBoxSizer* topBotSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* listMainSizer = new wxBoxSizer(wxHORIZONTAL);

	topBotSizer->Add(toolBar, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 5);
	topBotSizer->Add(listMainSizer, 1, wxEXPAND | wxALL, 5);

	listMainSizer->Add(listBar, 0, wxEXPAND);
	listMainSizer->Add(mainScreen, 1, wxEXPAND);

	this->SetSizerAndFit(topBotSizer);

	// ToolBar Panel section
	wxButton* buttonAdd = loadAndDisplayButtons(toolBar, BUTTON_ID_ADD, "icons/add.png","Add Matrix", wxPoint(0, 0));
	wxButton* buttonDelete = loadAndDisplayButtons(toolBar, BUTTON_ID_DELETE,"icons/minus.png", "Delete Matrix", wxPoint(100,0));
	wxButton* buttonCopy = loadAndDisplayButtons(toolBar, BUTTON_ID_COPY ,"icons/copy.png", "Copy Matrix", wxPoint(200, 0));
	wxButton* buttonEdit = loadAndDisplayButtons(toolBar, BUTTON_ID_EDIT ,"icons/edit.png", "Edit Matrix", wxPoint(300, 0));
	wxButton* buttonMatFunc = loadAndDisplayButtons(toolBar, BUTTON_ID_MATFUNC ,"icons/mat_func.png", "Mathematical Functions", wxPoint(400, 0));
	wxButton* buttonDrop = loadAndDisplayButtons(toolBar, BUTTON_ID_DROP ,"icons/delete.png", "Drop Matrix", wxPoint(500, 0));

	// ListBar Panel section
	wxListView* listView = new wxListView(listBar, wxID_ANY, wxDefaultPosition, wxSize(-1, 900));
	listView->AppendColumn("ID");
	listView->SetColumnWidth(0, 80);
	listView->AppendColumn("Name");
	listView->SetColumnWidth(1, 140);
	listView->AppendColumn("Size");
	listView->SetColumnWidth(2, 80);

	loadListView(listView);


	CreateStatusBar();
}

// Function loads the incons and displays button
wxButton* MainFrame::loadAndDisplayButtons(wxPanel* toolBar, const wxWindowID id, const wxString& imagePath, const wxString& label, const wxPoint& pointPosition) {
	
	// Looking for a imgHandler
	if (!wxImage::FindHandler(wxBITMAP_TYPE_PNG)){
		wxImage::AddHandler(new wxPNGHandler);
	}

	// Loading icons
	wxImage image;
	if (!image.LoadFile(imagePath, wxBITMAP_TYPE_PNG)){
		wxLogError("Nie mo¿na wczytaæ obrazka z pliku '%s'.", imagePath);
		return nullptr;
	}

	// Scaling Icons
	image = image.Scale(70, 70);

	//Creating Buttons
	wxButton* button = new wxButton(toolBar, id, label, pointPosition, wxDefaultSize, wxBU_NOTEXT);
	button->SetBitmap(image);
	button->SetBitmapMargins(10, 10);
	button->SetSize(button->GetBestSize());

	return button;
}

// Function loads the data from vector containing information about already added Matraces
void MainFrame::loadListView(wxListView* listView) {
	//TODO: load data from MatrixCalc
	listView->InsertItem(0, "1");
	listView->SetItem(0, 1, "Matrix A");
	listView->SetItem(0, 2, "2x2");
}

// Event listner on button clicked "ADD"
void MainFrame::OnButtonClickedAdd(wxCommandEvent& evt) {
	wxLogStatus("Add function initialized");
};

void MainFrame::OnButtonClickedDelete(wxCommandEvent& evt) {
	wxLogStatus("Delete function initialized");
};

void MainFrame::OnButtonClickedCopy(wxCommandEvent& evt) {
	wxLogStatus("Copy function initialized");
};

void MainFrame::OnButtonClickedEdit(wxCommandEvent& evt) {
	wxLogStatus("Edit function initialized");
};

void MainFrame::OnButtonClickedMatFunc(wxCommandEvent& evt) {
	wxLogStatus("MatFunc function initialized");
};

void MainFrame::OnButtonClickedDrop(wxCommandEvent& evt) {
	wxLogStatus("Drop function initialized");
};