#pragma once
#include <wx/wx.h>
#include <wx/listctrl.h>

class MainFrame : public wxFrame{
public:
	MainFrame(const wxString& title);
private:
	wxButton* loadAndDisplayButtons(wxPanel* toolBar, const wxWindowID id, const wxString& imagePath, const wxString& label, const wxPoint& pointPosition);
	void loadListView(wxListView* listView);
	
	//Button init section
	void OnButtonClickedAdd(wxCommandEvent& evt);
	void OnButtonClickedDelete(wxCommandEvent& evt);
	void OnButtonClickedCopy(wxCommandEvent& evt);
	void OnButtonClickedEdit(wxCommandEvent& evt);
	void OnButtonClickedMatFunc(wxCommandEvent& evt);
	void OnButtonClickedDrop(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
};