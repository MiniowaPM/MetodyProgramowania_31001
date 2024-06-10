#pragma once
#include <wx/listctrl.h>
#include "MatrixEquations.h"
#include <wx/wx.h>
#include <wx/grid.h>
#include "Globals.h"

class MainFrame : public wxFrame{
public:
	MainFrame(const wxString& title);
private:
    void loadListView();
    wxButton* loadAndDisplayButtons(wxPanel* toolBar, const wxWindowID id, const wxString& imagePath, const wxString& label, const wxPoint& pointPosition);
    void OnButtonClickedCreate(wxCommandEvent& evt);
    void OnButtonClickedDelete(wxCommandEvent& evt);
    void OnButtonClickedCopy(wxCommandEvent& evt);
    void OnButtonClickedEdit(wxCommandEvent& evt);
    void OnButtonClickedMatFunc(wxCommandEvent& evt);
    void OnButtonClickedDrop(wxCommandEvent& evt);
    void OnButtonClickedSaveMatrix(wxCommandEvent& evt);
    void OnButtonClickedImport(wxCommandEvent& evt);
    void OnButtonClickedExport(wxCommandEvent& evt);
    // Create Input objects
    void CreateSingleMatrixInput();
    void CreateDualMatrixInput();
    void CreateIntInput();
    void CreateFloatInput();
    void CreateSubmitDataButton();
    // Create Input Panels
    void DisplayEquationOptions(wxCommandEvent& evt);
    void DisplayReadDataPanel(Matrix& matrix);
    // Clear mainPanel Functions
    void ClearMatrixInputs();
    void ClearMainPanels();
    // Display Matrix Equations Result
    void DisplayAddMatrixResult(wxCommandEvent& evt); 
    void DisplaySubMatrixResult(wxCommandEvent& evt);
    void DisplayMultiplyByMatrixResult(wxCommandEvent& evt);
    void DisplayMultiplyByValueResult(wxCommandEvent& evt);
    void DisplayPowerOfMatrixResult(wxCommandEvent& evt);
    void DisplayTransposeResult(wxCommandEvent& evt);
    void DisplayDeterminantResult(wxCommandEvent& evt);
    void DisplayInverseResult(wxCommandEvent& evt);
    void DisplayRankResult(wxCommandEvent& evt);
    // Display Equation Result functions
    void DisplayMatrixResult(Matrix matrixResult);
    void QuickSaveHandler(wxCommandEvent& evt);
    void DisplayValueResult(float FloatResult, std::string matrixName);
    void DisplayValueResult(int IntResult, std::string matrixName);
    // Import Export handlers
    void ExportMatrixToFile(std::string fileName);
    void ImportMatrixToFile(std::string filePath);
    // Validator and Error handlers
    void ErrorMessageHandler(std::string errorMessage);

    MatrixEquations matrixEquations;
    wxPanel* mainScreenLeft;
    wxPanel* mainScreenRight;
    wxChoice* mainChoiceBox;
    wxListView* listView;
    wxGrid* grid;
    wxChoice* matrixEquationInputChoice;
    wxBoxSizer* choicesPanelSizer;
    wxChoice* matrixInputChoiceFirst;
    wxChoice* matrixInputChoiceSecound;
    wxTextCtrl* intValueInput;
    wxTextCtrl* floatValueInput;
    wxButton* saveDataButton;
    wxBoxSizer* ResultPanelSizer;
    Matrix resultMatrix;
    wxTextCtrl* newMatrixName;
    wxDECLARE_EVENT_TABLE();
};
