#include "MainFrame.h"
#include "MatrixEquations.h"
#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/choice.h>
#include <wx/valnum.h>
#include <wx/gdicmn.h> // wxSize
#include <wx/image.h> // imageHandler
#include <wx/dialog.h>
#include <vector>
#include <wx/grid.h>
#include "Globals.h"
#include <fstream>
#include <sstream>

enum ButtonIDs {
	BUTTON_ID_ADD = 2,
	BUTTON_ID_DELETE = 3, 
	BUTTON_ID_COPY = 4,
	BUTTON_ID_EDIT = 5,
	BUTTON_ID_MATFUNC = 6,
	BUTTON_ID_DROP = 7,
	BUTTON_ID_SAVE = 8,
	BUTTON_ID_SAVE_CHOISEBOX = 9
};

enum PanelIDs {
	PANEL_ID_TOOLBAR = 11,
	PANEL_ID_LISTBAR = 12,
	PANEL_ID_MAINSCREEN_RIGHT = 13,
	PANEL_ID_MAINSCREEN_LEFT = 14
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(BUTTON_ID_ADD, MainFrame::OnButtonClickedCreate)
EVT_BUTTON(BUTTON_ID_DELETE, MainFrame::OnButtonClickedDelete)
EVT_BUTTON(BUTTON_ID_COPY, MainFrame::OnButtonClickedCopy)
EVT_BUTTON(BUTTON_ID_EDIT, MainFrame::OnButtonClickedEdit)
EVT_BUTTON(BUTTON_ID_MATFUNC, MainFrame::OnButtonClickedMatFunc)
EVT_BUTTON(BUTTON_ID_DROP, MainFrame::OnButtonClickedDrop)
EVT_BUTTON(BUTTON_ID_SAVE, MainFrame::OnButtonClickedSaveMatrix)

wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title), grid(nullptr){

	CreateStatusBar();

	// Panel layout and styling section
	wxPanel* toolBar = new wxPanel(this, PANEL_ID_TOOLBAR, wxDefaultPosition, wxSize(200, 100));
	toolBar->SetBackgroundColour(wxColor(100, 100, 200));

	wxPanel* listBar = new wxPanel(this, PANEL_ID_LISTBAR, wxDefaultPosition, wxSize(300, 100));
	listBar->SetBackgroundColour(wxColor(200, 100, 100));

	mainScreenLeft = new wxPanel(this, PANEL_ID_MAINSCREEN_LEFT, wxDefaultPosition, wxSize(200, 100));
	mainScreenLeft->SetBackgroundColour(wxColor(200, 200, 100));

	mainScreenRight = new wxPanel(this, PANEL_ID_MAINSCREEN_RIGHT, wxDefaultPosition, wxSize(200, 100));
	mainScreenRight->SetBackgroundColour(wxColor(100, 200, 100));

	// Creating basic sizers layout
	wxBoxSizer* topBotSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* listMainSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	topBotSizer->Add(toolBar, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 5);
	topBotSizer->Add(listMainSizer, 1, wxEXPAND | wxALL, 5);

	listMainSizer->Add(listBar, 0, wxEXPAND);
	listMainSizer->Add(mainSizer, 1, wxEXPAND);

	mainSizer->Add(mainScreenLeft, 1, wxEXPAND);
	mainSizer->Add(mainScreenRight, 3, wxEXPAND);


	this->SetSizerAndFit(topBotSizer);

	// ToolBar Panel section
	wxButton* buttonAdd = loadAndDisplayButtons(toolBar, BUTTON_ID_ADD, "icons/add.png", "Add Matrix", wxPoint(0, 0));
	wxButton* buttonDelete = loadAndDisplayButtons(toolBar, BUTTON_ID_DELETE, "icons/minus.png", "Delete Matrix", wxPoint(100, 0));
	wxButton* buttonCopy = loadAndDisplayButtons(toolBar, BUTTON_ID_COPY, "icons/copy.png", "Copy Matrix", wxPoint(200, 0));
	wxButton* buttonEdit = loadAndDisplayButtons(toolBar, BUTTON_ID_EDIT, "icons/edit.png", "Edit Matrix", wxPoint(300, 0));
	wxButton* buttonMatFunc = loadAndDisplayButtons(toolBar, BUTTON_ID_MATFUNC, "icons/mat_func.png", "Mathematical Functions", wxPoint(400, 0));
	wxButton* buttonDrop = loadAndDisplayButtons(toolBar, BUTTON_ID_DROP, "icons/delete.png", "Drop Matrix", wxPoint(500, 0));
	wxButton* buttonImport = loadAndDisplayButtons(toolBar, wxID_ANY, "icons/import.png", "Import Matrix", wxPoint(600, 0));
	wxButton* buttonExport = loadAndDisplayButtons(toolBar, wxID_ANY, "icons/export.png", "Export Matrix", wxPoint(700, 0));

	buttonImport->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClickedImport, this);
	buttonExport->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClickedExport, this);

	// ListBar Panel section
	listView = new wxListView(listBar, wxID_ANY, wxDefaultPosition, wxSize(-1, 900));
	listView->AppendColumn("ID");
	listView->SetColumnWidth(0, 80);
	listView->AppendColumn("Name");
	listView->SetColumnWidth(1, 140);
	listView->AppendColumn("Size");
	listView->SetColumnWidth(2, 80);

	loadListView();

};

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
void MainFrame::loadListView() {

	listView->DeleteAllItems();

	int counter = 0;
	if (Matrices.empty()) {
		wxLogStatus("No matrices found");
	}
	else {
		for (auto& matrix : Matrices) {
			++counter;
			wxString MatrixName(matrix.name);
			wxString MatrixSize(wxString::Format("Size: %dx%d", matrix.rows, matrix.cols));
			int itemIndex = listView->InsertItem(counter, wxString::Format("%d", counter));			
			listView->SetItem(itemIndex,1, MatrixName);
			listView->SetItem(itemIndex, 2, MatrixSize);
		}
		wxLogStatus("Matrices loaded");
	}
}

// Event listners on button clicked
void MainFrame::OnButtonClickedCreate(wxCommandEvent& evt) {
	wxLogStatus("Create function initialized");
	ClearMainPanels();
	wxTextEntryDialog dialog(this, "Enter matrix name: ", "Create matrix", "Matrix name");
	if (dialog.ShowModal() == wxID_OK) {
		wxString newMatrixNameTemp = dialog.GetValue();
		wxTextEntryDialog dialog(this, "Enter matrix rows: ", "Create matrix", "1");
		if (dialog.ShowModal() == wxID_OK) {
			wxString newMatrixRowsTemp = dialog.GetValue();
			wxTextEntryDialog dialog(this, "Enter matrix cols: ", "Create matrix", "1");
			if (dialog.ShowModal() == wxID_OK) {
				wxString newMatrixColsTemp = dialog.GetValue();
				// Converting data from wxString
				std::string newMatrixName = std::string(newMatrixNameTemp.mb_str());
				/*if (isMatrixNameTaken(newMatrixName)){*/
				if (matrixEquations.getMatrixByName(newMatrixName) == nullptr) {
					int newMatrixRows = wxAtoi(newMatrixRowsTemp);
					int newMatrixCols = wxAtoi(newMatrixColsTemp);
					Matrix newMatrix = matrixEquations.createMatrix(newMatrixName, newMatrixRows, newMatrixCols);
					selectedMatrix = matrixEquations.getMatrixByName(newMatrixName);
					DisplayReadDataPanel(newMatrix);
					wxLogStatus("Matrix has been created");
					loadListView();
				}
				else {
					std::string errorMess = newMatrixName + " is already taken.";
					ErrorMessageHandler(errorMess);
				}
			}
		}
	}
	else {
		wxLogStatus("The operation was interrupted");
	};
};
void MainFrame::OnButtonClickedDelete(wxCommandEvent& evt) {
	wxLogStatus("Delete function initialized");
	ClearMainPanels();
	auto result = wxMessageBox("Do you want to delete this Matrix", "Deleting matrix", wxOK | wxCANCEL | wxICON_INFORMATION);
	if (result == wxOK){
		long selectedItem = -1;
		if (listView != nullptr) {
			if (listView->GetItemCount() > 0) {
				selectedItem = listView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
				if (selectedItem == -1) {
					wxLogStatus("No item is currently selected.");
				}
				else {
					wxLogStatus(wxString::Format("Selected item index: %ld", selectedItem));
					wxString matrixNameTemp = listView->GetItemText(selectedItem, 1);
					std::string matrixName = std::string(matrixNameTemp.mb_str());
					matrixEquations.deleteMatrix(matrixName);
					loadListView();
				}
			}
			else {
				wxLogStatus("listView is empty.");
			}
		}
		else {
			wxLogStatus("listView is not initialized.");
		}
	}
};
void MainFrame::OnButtonClickedCopy(wxCommandEvent& evt) {
	wxLogStatus("Copy function initialized");
	auto result = wxMessageBox("Do you want to copy this matrix", "Coping matrix", wxOK | wxCANCEL | wxICON_INFORMATION);
	if (result == wxOK) {
		long selectedItem = -1;
		if (listView != nullptr) {
			if (listView->GetItemCount() > 0) {
				selectedItem = listView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
				if (selectedItem == -1) {
					wxLogStatus("No item is currently selected.");
				}
				else {
					wxTextEntryDialog dialog(this, "Enter matrix name: ", "Copy Into matrix", "Matrix name");
					if (dialog.ShowModal() == wxID_OK) {
						wxString newMatrixNameInput = dialog.GetValue();
						std::string matrixBName = std::string(newMatrixNameInput.mb_str());
						if (matrixEquations.getMatrixByName(matrixBName) == nullptr){
							wxLogStatus(wxString::Format("Selected item index: %ld", selectedItem));
							wxString matrixNameInput = listView->GetItemText(selectedItem, 1);
							std::string matrixAName = std::string(matrixNameInput.mb_str());
							Matrix* matrixA = matrixEquations.getMatrixByName(matrixAName);
							selectedMatrix = matrixEquations.getMatrixByName(matrixAName);
							matrixEquations.copyMatrix(*matrixA, matrixBName);
						}
						else {
							std::string errorMess = matrixBName + " is already taken.";
							ErrorMessageHandler(errorMess);
						}
						loadListView();
					}
				}
			}
			else {
				wxLogStatus("listView is empty.");
			}
		}
		else {
			wxLogStatus("listView is not initialized.");
		}
	}
};
void MainFrame::OnButtonClickedEdit(wxCommandEvent& evt) {
	ClearMainPanels();
	wxLogStatus("Edit function initialized");
	auto result = wxMessageBox("Do you want to edit this Matrix?", "Editing matrix", wxOK | wxCANCEL | wxICON_INFORMATION);
	if (result == wxOK) {
		long selectedItem = -1;
		if (listView != nullptr) {
			if (listView->GetItemCount() > 0) {
				selectedItem = listView->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
				if (selectedItem == -1) {
					wxLogStatus("No item is currently selected.");
				}
				else {
					wxLogStatus(wxString::Format("Selected item index: %ld", selectedItem));
					wxString matrixNameTemp = listView->GetItemText(selectedItem, 1);
					std::string matrixName = std::string(matrixNameTemp.mb_str());
					Matrix* matrix = matrixEquations.getMatrixByName(matrixName);
					DisplayReadDataPanel(*matrix);
				}
			}
			else {
				wxLogStatus("listView is empty.");
			}
		}
		else {
			wxLogStatus("listView is not initialized.");
		}
	}
};
void MainFrame::OnButtonClickedSaveMatrix(wxCommandEvent& evt) {

	int rows = grid->GetNumberRows();
	int cols = grid->GetNumberCols();
	std::vector<std::vector<float>> newData(rows, std::vector<float>(cols));

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			newData[i][j] = std::stof(grid->GetCellValue(i, j).ToStdString());
		}
	}
	auto result = wxMessageBox("Do you want to submit this Matrix", "Submiting matrix", wxOK | wxCANCEL | wxICON_INFORMATION);
	if (result == wxOK) {

		selectedMatrix->data = newData;
		matrixEquations.insertIntoMatrix(*selectedMatrix, newData);

		ClearMainPanels();
	}
};
void MainFrame::DisplayReadDataPanel(Matrix& matrix) {
	std::vector<std::vector<float>> data;
	// MainPanel section
	ClearMainPanels();

	// Display matrix name and size in mainScreenRight
	std::string info = "Name: " + matrix.name + "\nSize: " +
		std::to_string(matrix.rows) + "x" +
		std::to_string(matrix.cols);
	new wxStaticText(mainScreenLeft, wxID_ANY, info, wxPoint(10, 10));

	//Display data in mainScreenRight
	grid = new wxGrid(mainScreenRight, wxID_ANY, wxDefaultPosition, wxSize(1200, 1100));
	grid->CreateGrid(matrix.rows, matrix.cols);

	for (int i = 0; i < matrix.rows; ++i) {
		for (int j = 0; j < matrix.cols; ++j) {
			grid->SetCellValue(i, j, std::to_string(matrix.data[i][j]));
			grid->SetCellEditor(i, j, new wxGridCellFloatEditor());
			grid->SetCellRenderer(i, j, new wxGridCellFloatRenderer(-1,2));
		}
	}

	wxBoxSizer* gridSizer = new wxBoxSizer(wxVERTICAL);
	gridSizer->Add(grid, 1, wxEXPAND | wxALL, 5);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(gridSizer, 1, wxEXPAND);

	mainScreenRight->SetSizer(mainSizer);

	wxButton* saveButton = new wxButton(mainScreenLeft, BUTTON_ID_SAVE, "Submit Matrix", wxDefaultPosition);

	wxBoxSizer* saveButtonSizer = new wxBoxSizer(wxVERTICAL);
	saveButtonSizer->AddStretchSpacer();
	saveButtonSizer->Add(saveButton, 0, wxALIGN_CENTER_HORIZONTAL);
	saveButtonSizer->AddStretchSpacer();
	mainScreenLeft->SetSizer(saveButtonSizer);

	Layout();
};
void MainFrame::OnButtonClickedMatFunc(wxCommandEvent& evt) {
	wxLogStatus("MatFunc function initialized");
	// MatFuncPanel Layout Section
	ClearMainPanels();

	// Input section
	wxArrayString matrixEquationChoises = { "addMatrix", "subtractMatrix", "multiplyMatrix", "multiplyMatrixByValue", "powerOfMatrix", "transposeMatrix", "detMatrix", "inverseMatrix", "rankMatrix"};
	matrixEquationInputChoice = new wxChoice(mainScreenLeft, wxID_ANY, wxDefaultPosition, wxDefaultSize, matrixEquationChoises);
	
		// Input Choices Styling Section
		choicesPanelSizer = new wxBoxSizer(wxVERTICAL);
		mainScreenLeft->SetSizer(choicesPanelSizer);

		wxBoxSizer* itemSizer = new wxBoxSizer(wxHORIZONTAL);
		choicesPanelSizer->Add(itemSizer, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	
		itemSizer->Add(matrixEquationInputChoice, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
		mainScreenLeft->SetSizerAndFit(choicesPanelSizer);


	matrixEquationInputChoice->Bind(wxEVT_CHOICE, &MainFrame::DisplayEquationOptions, this);

	Layout();
};
void MainFrame::OnButtonClickedDrop(wxCommandEvent& evt) {
	auto result = wxMessageBox("Do you want to delete EVERY matrix", "Deleting matrix", wxOK | wxCANCEL | wxICON_INFORMATION);
	if (result == wxOK) {
		Matrices.clear();
		loadListView();
	}
};
void MainFrame::OnButtonClickedImport(wxCommandEvent& evt) {
	wxLogStatus("Import function initialized");
	wxFileDialog openFileDialog(this, _("Open file"), "", "",
		"All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return; // the user changed their mind
	}
	wxString wxFilePath = openFileDialog.GetPath();
	std::string filePath = std::string(wxFilePath.c_str());
	ImportMatrixToFile(filePath);
};
void MainFrame::OnButtonClickedExport(wxCommandEvent& evt) {
	wxLogStatus("Export function initialized");
	wxTextEntryDialog dialog(this, "File name: ", "Export matrices", "Export1");
	if (dialog.ShowModal() == wxID_OK){
		wxString wxFileName = dialog.GetValue();
		std::string fileName = std::string(wxFileName.mb_str());
		ExportMatrixToFile(fileName);
	}
};

void MainFrame::CreateSingleMatrixInput() {
	wxArrayString matrixInputChoices;
	if (Matrices.empty()) {
		wxLogStatus("No matrices found");
	}
	else {
		for (auto& matrix : Matrices) {
			matrixInputChoices.Add(matrix.name);
		}
	}
	matrixInputChoiceFirst = new wxChoice(mainScreenLeft, wxID_ANY, wxDefaultPosition, wxDefaultSize, matrixInputChoices);
	// Input Choices Styling Section
		wxBoxSizer* itemSizer = new wxBoxSizer(wxHORIZONTAL);
		choicesPanelSizer->Add(itemSizer, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

		itemSizer->Add(matrixInputChoiceFirst, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
		mainScreenLeft->SetSizerAndFit(choicesPanelSizer);
};
void MainFrame::CreateDualMatrixInput() {
	wxArrayString matrixInputChoices;
	if (Matrices.empty()) {
		wxLogStatus("No matrices found");
	}
	else {
		for (auto& matrix : Matrices) {
			matrixInputChoices.Add(matrix.name);
		}
	}
	matrixInputChoiceFirst = new wxChoice(mainScreenLeft, wxID_ANY, wxDefaultPosition, wxDefaultSize, matrixInputChoices);
	matrixInputChoiceSecound = new wxChoice(mainScreenLeft, wxID_ANY, wxDefaultPosition, wxDefaultSize, matrixInputChoices);

	// Input Choices Styling Section
	wxBoxSizer* itemSizerFirst = new wxBoxSizer(wxHORIZONTAL);
	choicesPanelSizer->Add(itemSizerFirst, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	itemSizerFirst->Add(matrixInputChoiceFirst, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	mainScreenLeft->SetSizerAndFit(choicesPanelSizer);

	wxBoxSizer* itemSizerSec = new wxBoxSizer(wxHORIZONTAL);
	choicesPanelSizer->Add(itemSizerSec, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	itemSizerSec->Add(matrixInputChoiceSecound, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	mainScreenLeft->SetSizerAndFit(choicesPanelSizer);
};
void MainFrame::CreateIntInput() {
	wxIntegerValidator<int> intValidator;
	intValidator.SetRange(0, 100);

	intValueInput = new wxTextCtrl(mainScreenLeft, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, intValidator);

	wxBoxSizer* itemSizerInt = new wxBoxSizer(wxHORIZONTAL);
	choicesPanelSizer->Add(itemSizerInt, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	itemSizerInt->Add(intValueInput, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	mainScreenLeft->SetSizerAndFit(choicesPanelSizer);
};
void MainFrame::CreateFloatInput() {
	wxFloatingPointValidator<float> floatValidator;
	floatValidator.SetRange(-100.0f, 100.0f);
	floatValidator.SetPrecision(2);

	floatValueInput = new wxTextCtrl(mainScreenLeft, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, floatValidator);


	wxBoxSizer* itemSizerFloat = new wxBoxSizer(wxHORIZONTAL);
	choicesPanelSizer->Add(itemSizerFloat, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	itemSizerFloat->Add(floatValueInput, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	mainScreenLeft->SetSizerAndFit(choicesPanelSizer);
};
void MainFrame::ClearMatrixInputs() {
	wxWindowList& childrenLeft = mainScreenLeft->GetChildren();
	while (!childrenLeft.IsEmpty()) {
		wxWindow* child = childrenLeft.GetFirst()->GetData();
		if (child->GetId() != matrixEquationInputChoice->GetId()) {
			child->Destroy();
		}
		else {
			childrenLeft.Erase(childrenLeft.GetFirst());
		}
	}
	wxWindowList& childrenRight = mainScreenRight->GetChildren();
	while (!childrenRight.IsEmpty()) {
		wxWindow* child = childrenRight.GetFirst()->GetData();
		child->Destroy();
	}
};
void MainFrame::CreateSubmitDataButton() {
	// Creating new saveDataButton
	saveDataButton = new wxButton(mainScreenLeft, wxID_ANY, "Submit data", wxDefaultPosition);
	wxBoxSizer* itemSizer = new wxBoxSizer(wxHORIZONTAL);
	choicesPanelSizer->Add(itemSizer, 1, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	itemSizer->Add(saveDataButton, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	mainScreenLeft->SetSizerAndFit(choicesPanelSizer);
};

void MainFrame::DisplayEquationOptions(wxCommandEvent& evt) {
	wxLogStatus("Initialzied DisplayEquationOptions");
	int matrixEquationInputChoiseSelection = matrixEquationInputChoice->GetCurrentSelection();
	ClearMatrixInputs();
	switch (matrixEquationInputChoiseSelection){
	case 0: { // Add Matrix
		wxLogStatus("Add Matrix");
		CreateDualMatrixInput();
		CreateSubmitDataButton();
		saveDataButton->Bind(wxEVT_BUTTON, &MainFrame::DisplayAddMatrixResult, this);
		break;
	};
	case 1: { // Subtract Matrix
		wxLogStatus("Subtract Matrix");
		CreateDualMatrixInput();
		CreateSubmitDataButton();
		saveDataButton->Bind(wxEVT_BUTTON, &MainFrame::DisplaySubMatrixResult, this);
		break;
	};
	case 2: { // Multiply Matrix by Matrix
		wxLogStatus("Multiply Matrix by Matrix");
		CreateDualMatrixInput();
		CreateSubmitDataButton();
		saveDataButton->Bind(wxEVT_BUTTON, &MainFrame::DisplayMultiplyByMatrixResult, this);
		break;
	};
	case 3: { // Multiply Matrix by Value
		wxLogStatus("Multiply Matrix by Value");
		CreateSingleMatrixInput();
		CreateFloatInput();
		CreateSubmitDataButton();
		saveDataButton->Bind(wxEVT_BUTTON, &MainFrame::DisplayMultiplyByValueResult, this);
		break;
	};
	case 4: { // Power of Matrix
		wxLogStatus("Power of Matrix");
		CreateSingleMatrixInput(); 
		CreateIntInput();
		CreateSubmitDataButton();
		saveDataButton->Bind(wxEVT_BUTTON, &MainFrame::DisplayPowerOfMatrixResult, this);
		break;
	};
	case 5: { // Transpose Matrix
		wxLogStatus("Transpose Matrix");
		CreateSingleMatrixInput();
		CreateSubmitDataButton();
		saveDataButton->Bind(wxEVT_BUTTON, &MainFrame::DisplayTransposeResult, this);
		break;
	};
	case 6: { // Matrix determinant
		wxLogStatus("Matrix determinant");
		CreateSingleMatrixInput();
		CreateSubmitDataButton();
		saveDataButton->Bind(wxEVT_BUTTON, &MainFrame::DisplayDeterminantResult, this);
		break;
	};
	case 7: { // Inverse Matrix
		wxLogStatus("Inverse Matrix");
		CreateSingleMatrixInput();
		CreateSubmitDataButton();
		saveDataButton->Bind(wxEVT_BUTTON, &MainFrame::DisplayInverseResult, this);
		break;
	};
	case 8: { // Matrix rank
		wxLogStatus("Matrix rank");
		CreateSingleMatrixInput();
		CreateSubmitDataButton();
		saveDataButton->Bind(wxEVT_BUTTON, &MainFrame::DisplayRankResult, this);
		break;
	};
	default:
		wxLogStatus("Error: Invalid choice value");
		break;
	};
	Layout();
};

void MainFrame::DisplayAddMatrixResult(wxCommandEvent& evt) {
	wxLogStatus("Initialzied DisplayAddMatrixResult");

	wxString matrixAwxstring = matrixInputChoiceFirst->GetStringSelection();
	wxString matrixBwxstring = matrixInputChoiceSecound->GetStringSelection();

	std::string matrixAstring = matrixAwxstring.ToStdString();
	std::string matrixBstring = matrixBwxstring.ToStdString();

	Matrix* matrixA = matrixEquations.getMatrixByName(matrixAstring);
	Matrix* matrixB = matrixEquations.getMatrixByName(matrixBstring);

	if (matrixA != nullptr && matrixB != nullptr){
		Matrix matrixResult = matrixEquations.addMatrix(*matrixA, *matrixB);
		if (matrixResult.name != "matrixReadFail") {
			DisplayMatrixResult(matrixResult);
		}
		else {
			ErrorMessageHandler("Invalid matrix sizes. Sizes has to be equal.");
		}
	}
	else{
		ErrorMessageHandler("Invalid matrix value");
	}

};
void MainFrame::DisplaySubMatrixResult(wxCommandEvent& evt) {
	wxLogStatus("Initialzied DisplaySubMatrixResult");

	wxString matrixAwxstring = matrixInputChoiceFirst->GetStringSelection();
	wxString matrixBwxstring = matrixInputChoiceSecound->GetStringSelection();

	std::string matrixAstring = matrixAwxstring.ToStdString();
	std::string matrixBstring = matrixBwxstring.ToStdString();

	Matrix* matrixA = matrixEquations.getMatrixByName(matrixAstring);
	Matrix* matrixB = matrixEquations.getMatrixByName(matrixBstring);

	if (matrixA != nullptr && matrixB != nullptr) {
	Matrix matrixResult = matrixEquations.subtractMatrix(*matrixA, *matrixB);
		if (matrixResult.name != "matrixReadFail") {
			DisplayMatrixResult(matrixResult);
		}
		else {
			ErrorMessageHandler("Invalid matrix sizes. Sizes has to be equal.");
		}
	}
	else {
		ErrorMessageHandler("Invalid matrix value");
	}
};
void MainFrame::DisplayMultiplyByMatrixResult(wxCommandEvent& evt) {
	wxLogStatus("Initialzied DisplayMultiplyByMatrixResult");

	wxString matrixAwxstring = matrixInputChoiceFirst->GetStringSelection();
	wxString matrixBwxstring = matrixInputChoiceSecound->GetStringSelection();

	std::string matrixAstring = matrixAwxstring.ToStdString();
	std::string matrixBstring = matrixBwxstring.ToStdString();

	Matrix* matrixA = matrixEquations.getMatrixByName(matrixAstring);
	Matrix* matrixB = matrixEquations.getMatrixByName(matrixBstring);

	if (matrixA != nullptr && matrixB != nullptr) {
		Matrix matrixResult = matrixEquations.multiplyMatrix(*matrixA, *matrixB);
		if (matrixResult.name != "matrixReadFail") {
			DisplayMatrixResult(matrixResult);
		}
		else {
			ErrorMessageHandler("Invalid matrix sizes. First matrix column size has to be equal to the second matrix row size.");
		}
	}
	else {
			ErrorMessageHandler("Invalid matrix value");
	}
};
void MainFrame::DisplayMultiplyByValueResult(wxCommandEvent& evt) {
	wxLogStatus("Initialzied DisplayMultiplyByValueResult");
	wxString matrixAwxstring = matrixInputChoiceFirst->GetStringSelection();
	wxString floatValueString = floatValueInput->GetValue();

	std::string matrixAstring = matrixAwxstring.ToStdString();
	double doubleValue;
	if (!floatValueString.ToDouble(&doubleValue)) { doubleValue = 0.0; };

	Matrix* matrixA = matrixEquations.getMatrixByName(matrixAstring);
	float floatValue = (float)doubleValue;

	if (matrixA != nullptr) {
		Matrix matrixResult = matrixEquations.multiplyMatrix(*matrixA, floatValue);
		DisplayMatrixResult(matrixResult);
	}
	else {
		ErrorMessageHandler("Invalid matrix value");
	}
};
void MainFrame::DisplayPowerOfMatrixResult(wxCommandEvent& evt) {
	wxLogStatus("Initialzied DisplayPowerOfMatrixResult");
	wxString matrixAwxstring = matrixInputChoiceFirst->GetStringSelection();
	wxString intValueString = intValueInput->GetValue();

	std::string matrixAstring = matrixAwxstring.ToStdString();
	int intValue = wxAtoi(intValueString);

	Matrix* matrixA = matrixEquations.getMatrixByName(matrixAstring);

	if (matrixA != nullptr) {
		Matrix matrixResult = matrixEquations.powerOfMatrix(*matrixA, intValue);
		DisplayMatrixResult(matrixResult);
	}
	else {
		ErrorMessageHandler("Invalid matrix value");
	}
};
void MainFrame::DisplayTransposeResult(wxCommandEvent& evt) {
	wxLogStatus("Initialzied DisplayTransposeResult");
	wxString matrixAwxstring = matrixInputChoiceFirst->GetStringSelection();

	std::string matrixAstring = matrixAwxstring.ToStdString();

	Matrix* matrixA = matrixEquations.getMatrixByName(matrixAstring);

	if (matrixA != nullptr) {
		Matrix matrixResult = matrixEquations.transposeMatrix(*matrixA);
		DisplayMatrixResult(matrixResult);
	}
	else {
		ErrorMessageHandler("Invalid matrix value");
	}
};
void MainFrame::DisplayDeterminantResult(wxCommandEvent& evt) {
	wxLogStatus("Initialzied DisplayDeterminantResult");
	wxString matrixAwxstring = matrixInputChoiceFirst->GetStringSelection();

	std::string matrixAstring = matrixAwxstring.ToStdString();

	Matrix* matrixA = matrixEquations.getMatrixByName(matrixAstring);

	if (matrixA != nullptr) {
		float FloatResult = matrixEquations.detMatrix(*matrixA);
		if (FloatResult != 9999) {
			DisplayValueResult(FloatResult, matrixAstring);
		}
		else {
			ErrorMessageHandler("Invalid matrix sizes. Sizes has to be equal.");
		}
	}
	else {
		ErrorMessageHandler("Invalid matrix value");
	}
};
void MainFrame::DisplayInverseResult(wxCommandEvent& evt) {
	wxLogStatus("Initialzied DisplayInverseResult");
	wxString matrixAwxstring = matrixInputChoiceFirst->GetStringSelection();

	std::string matrixAstring = matrixAwxstring.ToStdString();

	Matrix* matrixA = matrixEquations.getMatrixByName(matrixAstring);

	if (matrixA != nullptr) {
		Matrix matrixResult = matrixEquations.inverseMatrix(*matrixA);
		if (matrixResult.name != "matrixReadFail") {
			DisplayMatrixResult(matrixResult);
		}
		else {
			ErrorMessageHandler("Invalid matrix sizes. Sizes has to be equal.");
		}
	}
	else {
		ErrorMessageHandler("Invalid matrix value");
	}
};
void MainFrame::DisplayRankResult(wxCommandEvent& evt) {
	wxLogStatus("Initialzied DisplayRankResult");
	wxString matrixAwxstring = matrixInputChoiceFirst->GetStringSelection();

	std::string matrixAstring = matrixAwxstring.ToStdString();

	Matrix* matrixA = matrixEquations.getMatrixByName(matrixAstring);

	if (matrixA != nullptr) {
		int intResult = matrixEquations.rankMatrix(*matrixA);
		DisplayValueResult(intResult, matrixAstring);
	}
	else {
		ErrorMessageHandler("Invalid matrix value");
	}
};

void MainFrame::DisplayMatrixResult(Matrix matrix) {
	//Display data in mainScreenRight

	grid = new wxGrid(mainScreenRight, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	grid->CreateGrid(matrix.rows, matrix.cols);
	for (int i = 0; i < matrix.rows; ++i) {
		for (int j = 0; j < matrix.cols; ++j) {
			grid->SetCellValue(i, j, std::to_string(matrix.data[i][j]));
			grid->SetCellEditor(i, j, new wxGridCellFloatEditor());
			grid->SetCellRenderer(i, j, new wxGridCellFloatRenderer(-1,2));
		}
	}

	newMatrixName = new wxTextCtrl(mainScreenRight, wxID_ANY);
	wxButton* saveButton = new wxButton(mainScreenRight, wxID_ANY, "Save");
	// Layout section
	wxBoxSizer* gridSizer = new wxBoxSizer(wxVERTICAL);
	gridSizer->Add(grid, 1, wxEXPAND | wxALL, 5);

	wxBoxSizer* textSizer = new wxBoxSizer(wxVERTICAL);
	textSizer->Add(newMatrixName, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
	buttonSizer->Add(saveButton, 0, wxALIGN_CENTER | wxALL, 5);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(gridSizer, 1, wxEXPAND);
	mainSizer->Add(textSizer, 0, wxEXPAND);
	mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER);

	mainScreenRight->SetSizer(mainSizer);

	saveButton->Bind(wxEVT_BUTTON, &MainFrame::QuickSaveHandler, this);

	resultMatrix = matrix;

	Layout();
};
void MainFrame::QuickSaveHandler(wxCommandEvent& evt) {

	wxString wxStringMatrixName = newMatrixName->GetValue();
	std::string newMatrixNameString = std::string(wxStringMatrixName.mb_str());
	if (matrixEquations.getMatrixByName(newMatrixNameString) == nullptr){
		matrixEquations.quickSaveMatrix(resultMatrix, newMatrixNameString);
	}
	else {
		std::string errorMess = newMatrixNameString + " is already taken.";
		ErrorMessageHandler(errorMess);
	}
	loadListView();
};
void MainFrame::DisplayValueResult(float FloatResult, std::string matrixName) {
	std::string displayInfo = "Name: " + matrixName + "\nResult: " + std::to_string(FloatResult);
	wxStaticText* resultData = new wxStaticText(mainScreenRight, wxID_ANY, displayInfo, wxDefaultPosition);
};
void MainFrame::DisplayValueResult(int IntResult, std::string matrixName) {
	std::string displayInfo = "Name: " + matrixName + "\nResult: " + std::to_string(IntResult);
	wxStaticText* resultData = new wxStaticText(mainScreenRight, wxID_ANY, displayInfo, wxDefaultPosition);
};
void MainFrame::ClearMainPanels() {
	if (matrixEquationInputChoice != nullptr) {
		matrixEquationInputChoice->Destroy();
		matrixEquationInputChoice = nullptr;
	}
	if (saveDataButton != nullptr) {
		saveDataButton->Destroy();
		saveDataButton = nullptr;
	}
	wxWindowList& childrenLeft = mainScreenLeft->GetChildren();
	while (!childrenLeft.IsEmpty()) {
		wxWindow* child = childrenLeft.GetFirst()->GetData();
		child->Destroy();
	}
	wxWindowList& childrenRight = mainScreenRight->GetChildren();
	while (!childrenRight.IsEmpty()) {
		wxWindow* child = childrenRight.GetFirst()->GetData();
		child->Destroy();
	}
};

void MainFrame::ExportMatrixToFile(std::string fileName) {
	std::ofstream file;
	file.open(fileName + ".txt");
	// Error handler
	if (!file.is_open()) {
		std::string errorMess = "Nie mo¿na otworzyæ pliku: " + fileName;
		ErrorMessageHandler(errorMess);
		return;
	}
	// Write Matrices into std::string
	for (Matrix& matrix : Matrices) {
		file << "Matrix name:" << matrix.name << "\n";
		file << matrix.cols << "\n";
		file << matrix.rows << "\n";
		for (int i = 0; i < matrix.rows; i++){
			for (int j = 0; j < matrix.cols; j++){
				file << matrix.data[i][j];
				if (j != matrix.cols - 1 or i != matrix.rows - 1){
					file << ',';
				}
			}
		}
		file << "\n";
	}
	file.close();
};
void MainFrame::ImportMatrixToFile(std::string filePath) {
	std::ifstream file(filePath);
	// Error handling
	if (!file.is_open()) {
		std::string errorMess = "Nie mo¿na otworzyæ pliku!";
		ErrorMessageHandler(errorMess);
		return;
	}
	std::string line;
	while (std::getline(file, line)){
		if (line.empty()) {
			continue;
		}
		if (line.substr(0, 12) == "Matrix name:") {
			// Znaleziono now¹ macierz
			std::string matrixName = line.substr(12);
			int numCols, numRows;
			std::vector<std::vector<float>> matrixData;

			if (!std::getline(file, line)) {
				std::string errorMess = "B³¹d podczas wczytywania liczby kolumn z pliku: " + filePath;
				ErrorMessageHandler(errorMess);
				return;
			}
			numCols = std::stoi(line);

			if (!std::getline(file, line)) {
				std::string errorMess = "B³¹d podczas wczytywania liczby wierszy z pliku: " + filePath;
				ErrorMessageHandler(errorMess);
				return;
			}
			numRows = std::stoi(line);
			if (!std::getline(file, line)) {
				std::string errorMess = "B³¹d podczas wczytywania danych macierzy z pliku: " + filePath;
				ErrorMessageHandler(errorMess);
				return;
			}
			std::stringstream stringstream(line);
			std::string cell;
			std::vector<float> rowData;
			int rowIndex = 0;
			int colIndex = 0;
			Matrix matrix(matrixName, numRows, numCols);
			while (std::getline(stringstream, cell, ',')) {
				float cellValue = std::stof(cell);
				matrix.data[rowIndex][colIndex] = cellValue;
				colIndex++;
				if (colIndex == numCols) {
					colIndex = 0;
					rowIndex++;
				}
			}
			Matrices.push_back(matrix);
		}
	}
	file.close();
	loadListView();
};

void MainFrame::ErrorMessageHandler(std::string errorMessage) {
	wxLogError("An error occurred: %s", errorMessage);
};
