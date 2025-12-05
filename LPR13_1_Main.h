#ifndef LPR13_1_MainH
#define LPR13_1_MainH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.DBGrids.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.DBCtrls.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.Buttons.hpp>

class TFormStudents : public TForm
{
__published:
    TButton *btnLoadPhoto;
    TButton *btnSavePhoto;
    TButton *btnDeletePhoto;
    TDBGrid *gridStudents;
    TImage *imgPhoto;
    TADOConnection *connStudents;
    TADOTable *tblStudents;
    TDataSource *dsStudents;
    TDBNavigator *navStudents;
    TOpenPictureDialog *dlgPhotoOpen;
	TAutoIncField *tblStudentsКод;
    TWideStringField *tblStudentsПрізвище;
    TDateTimeField   *tblStudentsДатаНародження;
    TWideStringField *tblStudentsГрупа;
    TBlobField       *tblStudentsФотографія;

    void __fastcall FormCreate(TObject *Sender);
    void __fastcall tblStudentsAfterScroll(TDataSet *DataSet);
    void __fastcall btnLoadPhotoClick(TObject *Sender);
    void __fastcall btnSavePhotoClick(TObject *Sender);
    void __fastcall btnDeletePhotoClick(TObject *Sender);

public:
    __fastcall TFormStudents(TComponent* Owner);
};

extern PACKAGE TFormStudents *FormStudents;

#endif

