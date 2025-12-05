#include <vcl.h>
#pragma hdrstop

#include "LPR13_1_Main.h"
#include <System.SysUtils.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.Imaging.pngimage.hpp>

#pragma package(smart_init)
#pragma resource "*.dfm"

TFormStudents *FormStudents;

static void LoadPhotoFromField(TFormStudents *form)
{
    form->imgPhoto->Picture = NULL;
    try
    {
        if (!form->tblStudents->Active || form->tblStudents->RecordCount == 0)
            return;
        if (form->tblStudentsФотографія->IsNull)
            return;
        TMemoryStream *ms = new TMemoryStream();
        try
        {
            form->tblStudentsФотографія->SaveToStream(ms);
            ms->Position = 0;
            TJPEGImage *jpg = new TJPEGImage();
            try
            {
                jpg->LoadFromStream(ms);
                form->imgPhoto->Picture->Assign(jpg);
                delete jpg;
            }
            catch (...)
            {
                delete jpg;
                form->imgPhoto->Picture = NULL;
            }
            delete ms;
        }
        catch (...)
        {
            delete ms;
            form->imgPhoto->Picture = NULL;
        }
    }
    catch (...)
    {
        form->imgPhoto->Picture = NULL;
    }
}

__fastcall TFormStudents::TFormStudents(TComponent* Owner)
    : TForm(Owner)
{
}

void __fastcall TFormStudents::FormCreate(TObject *Sender)
{
    try
    {
        connStudents->Connected = false;
        tblStudents->Active = false;
        UnicodeString dbPath =
            ExtractFilePath(Application->ExeName) + "Students.mdb";
        UnicodeString connStr =
            "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dbPath +
            ";Persist Security Info=False";
        connStudents->LoginPrompt = false;
        connStudents->ConnectionString = connStr;
        connStudents->Connected = true;
        tblStudents->Active = true;
        LoadPhotoFromField(this);
    }
    catch (Exception &e)
    {
        ShowMessage("Помилка підключення до бази.");
    }
}

void __fastcall TFormStudents::tblStudentsAfterScroll(TDataSet *DataSet)
{
    LoadPhotoFromField(this);
}

void __fastcall TFormStudents::btnLoadPhotoClick(TObject *Sender)
{
    if (!dlgPhotoOpen->Execute())
        return;
    UnicodeString fileName = dlgPhotoOpen->FileName;
    UnicodeString ext = LowerCase(ExtractFileExt(fileName));
    try
    {
        if (ext == ".jpg" || ext == ".jpeg" || ext == ".jfif")
        {
            TJPEGImage *jpg = new TJPEGImage();
            try
            {
                jpg->LoadFromFile(fileName);
                imgPhoto->Picture->Assign(jpg);
                delete jpg;
            }
            catch (...)
            {
                delete jpg;
                throw;
            }
        }
        else if (ext == ".png")
        {
            TPngImage *png = new TPngImage();
            try
            {
                png->LoadFromFile(fileName);
                imgPhoto->Picture->Assign(png);
                delete png;
            }
            catch (...)
            {
                delete png;
                throw;
            }
        }
        else
        {
            imgPhoto->Picture->LoadFromFile(fileName);
        }
    }
    catch (...)
    {
        ShowMessage("Не вдалося завантажити це зображення. "
                    "Спробуйте інший файл (краще JPG або BMP).");
        imgPhoto->Picture = NULL;
    }
}

void __fastcall TFormStudents::btnSavePhotoClick(TObject *Sender)
{
    if (!tblStudents->Active || tblStudents->RecordCount == 0)
        return;
    if (imgPhoto->Picture->Graphic == NULL)
    {
        ShowMessage("Немає фото для збереження.");
        return;
    }
    try
    {
        TJPEGImage *jpg = new TJPEGImage();
        TMemoryStream *ms = new TMemoryStream();
        try
        {
            jpg->Assign(imgPhoto->Picture->Graphic);
            jpg->CompressionQuality = 90;
            jpg->DIBNeeded();
            jpg->SaveToStream(ms);
            ms->Position = 0;
            tblStudents->Edit();
            tblStudentsФотографія->LoadFromStream(ms);
            tblStudents->Post();
            delete jpg;
            delete ms;
        }
        catch (...)
        {
            delete jpg;
            delete ms;
            throw;
        }
        LoadPhotoFromField(this);
    }
    catch (...)
    {
        ShowMessage("Не вдалося зберегти фото.");
        if (tblStudents->State == Data::Db::dsEdit ||
            tblStudents->State == Data::Db::dsInsert)
        {
            tblStudents->Cancel();
        }
    }
}

void __fastcall TFormStudents::btnDeletePhotoClick(TObject *Sender)
{
    if (!tblStudents->Active || tblStudents->RecordCount == 0)
        return;
    try
    {
        tblStudents->Edit();
        tblStudentsФотографія->Clear();
        tblStudents->Post();
        imgPhoto->Picture = NULL;
    }
    catch (...)
    {
        ShowMessage("Не вдалося видалити фото.");
        if (tblStudents->State == Data::Db::dsEdit ||
            tblStudents->State == Data::Db::dsInsert)
        {
            tblStudents->Cancel();
        }
    }
}

