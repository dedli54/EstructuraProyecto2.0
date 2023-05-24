#include <Windows.h>
#include "resource.h"
#include <iostream>
#include <stdlib.h>
#include <Commctrl.h>
#include <string>
#include <Commdlg.h>
#include <fstream>
#include <time.h>
#include <cstdlib>


using namespace std;
HINSTANCE instGlobal;
WNDPROC lpEditWndProc;
HWND hWnd;
HBITMAP bmp;
HBITMAP bmp2;

//Variables inicio
int cuantashay = 0;
char nomuser[15];
char nompass[15];
int nousuariosregistrados;
int usuarionoencontrado;
int contrasenaincorrecta;
int usuariorepetido;
char copiarfoto[500];
int azar = 0;
int clavevacunarepetido;
char vacunaaborrar[20];
char personaaborrar[30];
bool mostrar = false; //mostrarcarnetbinario
int curprepetido;
int rfcrepetido;

//Variables end

//Struct inicio
struct fecha
{
	char dia[3];
	char mes[3];
	char anio[5];
};
struct user
{
	char usuario[15];
	char contra[15];
	char appaterno[15];
	char apmaterno[15];
	char nombres[15];
	user* siguiente;
	user* anterior;
}; user* iniciox, * auxx, * finx = nullptr;
struct RegistroPersonas
{
	fecha fechaCita;
	char paterno[15];
	char materno[15];
	char nombres[30];
	char curp[19];
	char rfc[14];
	char calle[15];
	char colonia[15];
	char ciudad[15];
	char estado[15];
	char estadocivil[15];
	int pretelefono;
	int telefono;
	char sexo[10];
	char grupoocupa[15];
	char perfilriesgo[7];
	char foto[250]; //id ya sea ine, pasaporte, etc....
	int carnetid;
	RegistroPersonas* siguiente;
	RegistroPersonas* anterior;
};  RegistroPersonas* inicio, * aux, * fin = nullptr, *u;
struct RegistroVacunas
{
	char tipo[15];
	char marca[15];
	int nodosis;
	char clavevacuna[15];
	char descripcion[30];
	int precio;
	RegistroVacunas* siguiente;
	RegistroVacunas* anterior;
}; RegistroVacunas* inicioxx, * auxxx, * finxx = nullptr;
struct Carnet {
	fecha fechaDosis;
	fecha fechaprox;
	char curp[19];
	char clavevacuna[15];
	int nodosis;
	int lote;
	char CentroVacuna[10];
	char nombres[30];
	RegistroVacunas vacuna;
	Carnet* siguiente;
	Carnet* anterior;
}; Carnet* inicioxxx, * auxxxx, * finxxx = nullptr;

//Struct Final

//Funciones inicio
void AgregarUsuario(user* nuevo);
void AgregarVacuna(RegistroVacunas* nuevo);
void AgregarPersona(RegistroPersonas* nuevo);
void escribirUsuarios(); void leerUsuarios();
void escribirPersonas(); void leerPersonas();
void escribirVacunas(); void leerVacunas();
void BuscarUsuario(char usuario[50]); void BuscarContrasena(char usuario[50]);
void BuscarUsuarioRepetido(char usuario[50]);
void BuscarClaveVacunaRepetida(char usuario[15]);
int BusquedaBinaria(int izquierda, int derecha, int buscado);
void escribirCarnet(); void leerCarnet();
void AgregarCarnet(Carnet* nuevo);
void Clave(WPARAM wParam, HWND hWnd, int buscado);
//void secuencial(char nombres[30]);


int contarNombres();
void ArregloNombre(RegistroPersonas R[], int tamaño);
void quickSort(RegistroPersonas R[], int left, int right);
void ArregloCarnet(RegistroPersonas R[], int tamaño);
void OrdenarCarnet(RegistroPersonas R[], int may, int men);
void heapSort(RegistroPersonas R[], int may);
void escribirTxtNombre(WPARAM wParam, HWND hWnd, int contadorNOMBRE, RegistroPersonas* arreglo);
void escribirTxtCarnet(WPARAM wParam, HWND hWnd, int contadorNOMBRE, RegistroPersonas* arreglo);



void BuscarCurpRepetido(char usuario[19]);
void BuscarRfcRepedito(char usuario[19]);




//Funciones final

//Funciones WinAPi inicio

BOOL CALLBACK LOGIN(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK REGISTRARSE(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MyEditCallBckProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK REGISTROPERSONAS(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK REGISTROVACUNAS(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK BAJADEVACUNAS(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EDITARVACUNAS(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK LISTAVACUNAS(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK BORRARPERSONAS(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EDITARPERSONAS(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK LISTAPERSONAS(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK CARNET(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AGREGARCARNET(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK NUMEROCARNET(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK MAINMENU(HWND, UINT, WPARAM, LPARAM);
//Funciones WinAPi final

//Funciones WinAPi inicio code
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {

	leerUsuarios();
	leerPersonas();
	leerVacunas();
	leerCarnet();

	HWND hVentana1 = CreateDialog(hInst, MAKEINTRESOURCE(IDD_INICIARSESION), NULL, LOGIN);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	ShowWindow(hVentana1, cShow);

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

void menu(WPARAM wParam, HWND hwnd) {
	switch (LOWORD(wParam)) {
	case ID_REGISTRO_PERSONAS: {
		EndDialog(hwnd, 0);
		DialogBox(instGlobal, MAKEINTRESOURCE(IDD_REGISTROPERSONAS), hwnd, REGISTROPERSONAS);
	}break;
	case ID_REGISTRO_VACUNAS: {
		EndDialog(hwnd, 0);
		DialogBox(instGlobal, MAKEINTRESOURCE(IDD_REGISTROVACUNAS), hwnd, REGISTROVACUNAS);
	}break;
	case ID_ELIMINAR_VACUNAS: {
		EndDialog(hwnd, 0);
		DialogBox(instGlobal, MAKEINTRESOURCE(IDD_BAJADEVACUNAS), hwnd, BAJADEVACUNAS);
	}break;
	case ID_MODIFICAR_VACUNAS: {
		EndDialog(hwnd, 0);
		DialogBox(instGlobal, MAKEINTRESOURCE(IDD_EDITARVACUNAS), hwnd, EDITARVACUNAS);
	}break;
	case ID_ELIMINAR_PERSONAS: {
		EndDialog(hwnd, 0);
		DialogBox(instGlobal, MAKEINTRESOURCE(IDD_BORRARPERSONAS), hwnd, BORRARPERSONAS);
	}break;
	case ID_MODIFICAR_PERSONAS: {
		EndDialog(hwnd, 0);
		DialogBox(instGlobal, MAKEINTRESOURCE(IDD_EDITARPERSONAS), hwnd, EDITARPERSONAS);
	}break;
	case ID_LISTA_PERSONAS: {
		EndDialog(hwnd, 0);
		DialogBox(instGlobal, MAKEINTRESOURCE(IDD_LISTAPERSONAS), hwnd, LISTAPERSONAS);
	}break;
	case ID_LISTA_VACUNAS: {
		EndDialog(hwnd, 0);
		DialogBox(instGlobal, MAKEINTRESOURCE(IDD_LISTVACUNAS), hwnd, LISTAVACUNAS);
	}break;
	case ID_CARNET_NUMERO: {
		EndDialog(hwnd, 0);
		DialogBox(instGlobal, MAKEINTRESOURCE(IDD_NUMEROCARNET), hwnd, NUMEROCARNET);
	}break;
	case ID_SALIR: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;
	
	case ID_MAINMENU: {
		EndDialog(hwnd, 0);
		DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MAIN_MENU), hwnd, MAINMENU);
	}break;
	case ID_REGISTRAR_CARNET: {
		EndDialog(hwnd, 0);
		DialogBox(instGlobal, MAKEINTRESOURCE(IDD_CARNET), hwnd, CARNET);
	}break;
	case ID_REPORTE_CARNET: {
		int contadorCARNET = contarNombres();
		RegistroPersonas* arreglo = new RegistroPersonas[contadorCARNET]; //mi puntero es igual a mi arreglo de carnet
		ArregloCarnet(arreglo, contadorCARNET);
		heapSort(arreglo, contadorCARNET);
		escribirTxtCarnet(wParam, hWnd, contadorCARNET, arreglo);
		delete[] arreglo;
		MessageBox(hWnd, "Reporte carnet guardado", "AVISO", MB_OK | MB_ICONINFORMATION);

	}break;
	case ID_REPORTE_NOMBRE: {
		int contadorNOMBRE = contarNombres();
		RegistroPersonas* arregloquick = new RegistroPersonas[contadorNOMBRE]; //mi puntero es igual a mi arreglo de carnet
		ArregloNombre(arregloquick, contadorNOMBRE);
		quickSort(arregloquick, 0, contadorNOMBRE - 1);
		escribirTxtNombre(wParam, hWnd, contadorNOMBRE, arregloquick);
		delete[] arregloquick;
		MessageBox(hWnd, "Reporte nombres guardado", "AVISO", MB_OK | MB_ICONINFORMATION);
	}break;
	}
}

BOOL CALLBACK LOGIN(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	case WM_CLOSE:
	{
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;
	case WM_DESTROY:
	{
		PostQuitMessage(117);
	}break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case BTN_INIC_REGISTRARSE:
		{
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_REGISTRARSE), hwnd, REGISTRARSE);
		}break;
		case BTN_INIC_INICIARSESION:
		{
			GetDlgItemText(hwnd, INICIARSESION_USUARIO, nomuser, 15);
			GetDlgItemText(hwnd, INICIARSESION_CONTR, nompass, 15);
			BuscarUsuario(nomuser);
			BuscarContrasena(nompass);

			if (nousuariosregistrados == 0)
			{
				MessageBox(hwnd, "No hay usuarios registrados", "AVISO", MB_OK | MB_ICONERROR);
			}
			else if (usuarionoencontrado == 0)
			{
				MessageBox(hwnd, "Usuario incorrecto", "AVISO", MB_OK | MB_ICONERROR);
			}
			else if (contrasenaincorrecta == 0)
			{
				MessageBox(hwnd, "Contrasena incorrecta", "AVISO", MB_OK | MB_ICONERROR);
			}
			else
			{
				EndDialog(hwnd, 0);
				DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MAIN_MENU), hwnd, MAINMENU);
			}
		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK REGISTRARSE(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	user* nodous = new user;
	switch (msg) {
	case WM_INITDIALOG: {
		lpEditWndProc = (WNDPROC)SetWindowLong(GetDlgItem(hwnd, BTN_REGISTRO_USUARIO), GWL_WNDPROC, (LONG)MyEditCallBckProcedure);

	}break;
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case BTN_REGISTRARSE_REGISTRO: {
			GetDlgItemText(hwnd, BTN_REGISTRO_USUARIO, nodous->usuario, 15);
			GetDlgItemText(hwnd, BTN_REGISTRO_CONTRA, nodous->contra, 15);
			GetDlgItemText(hwnd, BTN_REGISTRO_NOMBRES, nodous->nombres, 15);
			GetDlgItemText(hwnd, BTN_REGISTRO_APMATERNO, nodous->apmaterno, 15);
			GetDlgItemText(hwnd, BTN_REGISTRO_APPATERNO, nodous->appaterno, 15);
			BuscarUsuarioRepetido(nodous->usuario);
			if (usuariorepetido == 0) {
				AgregarUsuario(nodous);
				MessageBox(hwnd, "Usuario registrado.", "AVISO", MB_OK | MB_ICONINFORMATION);
				EndDialog(hwnd, 0);
				DialogBox(instGlobal, MAKEINTRESOURCE(IDD_INICIARSESION), hwnd, LOGIN);
			}
			else {
				MessageBox(hwnd, "Usuario repetido.", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		}
	}

	}
	return FALSE;
}

LRESULT CALLBACK MyEditCallBckProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (wParam == 32)
	{
		MessageBox(hWnd, "No se aceptan espacios.", "AVISO", MB_OK | MB_ICONSTOP);

		return 0;
	}


	return CallWindowProc(lpEditWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK REGISTROPERSONAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		lpEditWndProc = (WNDPROC)SetWindowLong(GetDlgItem(hwnd, IDC_REGISTROPERSONAS_CURP), GWL_WNDPROC, (LONG)MyEditCallBckProcedure);
		lpEditWndProc = (WNDPROC)SetWindowLong(GetDlgItem(hwnd, IDC_REGISTROPERSONAS_RFC), GWL_WNDPROC, (LONG)MyEditCallBckProcedure);

	}break;
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		menu(wParam, hwnd);
		switch (LOWORD(wParam)) {
		case IDC_REGISTROPERSONAS_REGISTRARBTN: {
			RegistroPersonas* nodouse = new RegistroPersonas;
			SYSTEMTIME fechaCita;
			ZeroMemory(&fechaCita, sizeof(fechaCita));
			SendDlgItemMessage(hwnd, IDC_REGISTROPERSONAS_DATEPICK, DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaCita);
			sprintf_s(nodouse->fechaCita.dia, "%d", fechaCita.wDay);
			sprintf_s(nodouse->fechaCita.mes, "%d", fechaCita.wMonth);
			sprintf_s(nodouse->fechaCita.anio, "%d", fechaCita.wYear);

			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_PATERNO, nodouse->paterno, 15);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_MATERNO, nodouse->materno, 15);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_NOMBRES, nodouse->nombres, 30);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_CURP, nodouse->curp, 19);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_RFC, nodouse->rfc, 14);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_CALLE, nodouse->calle, 15);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_COLONIA, nodouse->colonia, 15);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_CIUDAD, nodouse->ciudad, 15);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_ESTADO, nodouse->estado, 15);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_ESTADOCIVIL, nodouse->estadocivil, 15);
			strcpy_s(nodouse->foto, copiarfoto);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_SEXO, nodouse->sexo, 10);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_GRUPOOCUPA, nodouse->grupoocupa, 15);
			GetDlgItemText(hwnd, IDC_REGISTROPERSONAS_PERFILRIESGO, nodouse->perfilriesgo, 7);
			nodouse->telefono = GetDlgItemInt(hwnd, IDC_REGISTROPERSONAS_TELEFONO, NULL, FALSE);
			nodouse->pretelefono = GetDlgItemInt(hwnd, IDC_REGISTROPERSONAS_PRETELEFONO, NULL, FALSE);
			azar = rand() % 1000;
			aux = inicio;
			while (aux != nullptr) {
				if (aux->carnetid == azar) {
					azar = rand() % 1000;
				}
				aux = aux->siguiente;
			}
			nodouse->carnetid = azar;
			BuscarCurpRepetido(nodouse->curp);
			BuscarRfcRepedito(nodouse->rfc);
			if (curprepetido == 0 && rfcrepetido == 0) {
				AgregarPersona(nodouse);
				MessageBox(hwnd, "Persona registrada.", "AVISO", MB_OK | MB_ICONINFORMATION);
			}
			else {
				MessageBox(hwnd, "Curp o Rfc repetido.", "AVISO", MB_OK | MB_ICONERROR);
			}
			cuantashay++;
		}break;
		case IDC_REGISTROPERSONAS_CARGARFOTOBTN: {
			OPENFILENAME ofn;

			char zFile[250]; //260 caracteres

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.lpstrFileTitle = copiarfoto;
			ofn.nMaxFileTitle = sizeof(copiarfoto);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = copiarfoto;

			if (GetOpenFileName(&ofn) == TRUE) {
				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 150, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_REGISTROPERSONAS_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}
			else {
				MessageBox(hwnd, "No seleccionó una imagen", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK REGISTROVACUNAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	RegistroVacunas* nodouse = new RegistroVacunas;
	switch (msg) {
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		menu(wParam, hwnd);
		switch (LOWORD(wParam)) {
		case BTN_REGISTROVACUNAS_REGISTRAR: {
			nodouse->nodosis = GetDlgItemInt(hwnd, IDC_REGISTRARVACUNA_NODOSIS, NULL, FALSE);
			nodouse->precio = GetDlgItemInt(hwnd, IDC_REGISTRARVACUNA_PRECIO, NULL, FALSE);
			GetDlgItemText(hwnd, IDC_REGISTRARVACUNA_TIPO, nodouse->tipo, 15);
			GetDlgItemText(hwnd, IDC_REGISTRARVACUNA_MARCA, nodouse->marca, 15);
			GetDlgItemText(hwnd, IDC_REGISTRARVACUNA_CLAVE, nodouse->clavevacuna, 15);
			GetDlgItemText(hwnd, IDC_REGISTRARVACUNA_DESCRIPCION, nodouse->descripcion, 30);
			BuscarClaveVacunaRepetida(nodouse->clavevacuna);
			if (nodouse->precio > 0 && clavevacunarepetido == 0) {
				AgregarVacuna(nodouse);
				MessageBox(hwnd, "Vacuna registrada.", "AVISO", MB_OK | MB_ICONINFORMATION);
			}
			else {
				MessageBox(hwnd, "Precio debe ser mayor a 0 y la clave debe ser diferente.", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK BAJADEVACUNAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_INITDIALOG: {
		auxxx = inicioxx;
		while (auxxx != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_BAJAVACUNA_LISTA), LB_INSERTSTRING, 0, (LPARAM)auxxx->clavevacuna);
			auxxx = auxxx->siguiente;
		}
	}break;
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		menu(wParam, hwnd);
		switch (LOWORD(wParam)) {
		case IDC_BAJAVACUNA_LISTA: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[20] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_BAJAVACUNA_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_BAJAVACUNA_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				auxxx = inicioxx;
				while (auxxx->siguiente != nullptr && strcmp(textolist, auxxx->clavevacuna) != 0) {
					auxxx = auxxx->siguiente;
				}
				strcpy_s(vacunaaborrar, textolist);
				SetDlgItemText(hwnd, IDC_BAJAVACUNAS_TIPO, auxxx->tipo);
				SetDlgItemText(hwnd, IDC_BAJAVACUNAS_MARCA, auxxx->marca);
				SetDlgItemText(hwnd, IDC_BAJAVACUNAS_CLAVEVACUNA, auxxx->clavevacuna);
				SetDlgItemText(hwnd, IDC_BAJAVACUNAS_DESCRIPCION, auxxx->descripcion);
				SetDlgItemInt(hwnd, IDC_BAJAVACUNAS_NODOSIS, (UINT)auxxx->nodosis, FALSE);
				SetDlgItemInt(hwnd, IDC_BAJAVACUNAS_PRECIO, (UINT)auxxx->precio, FALSE);

			}break;
			}
		}break;
		case IDC_BAJAVACUNAS_BORRARBTN: {
			auxxx = inicioxx;
			RegistroVacunas* ant = nullptr;
			while (auxxx != nullptr && strcmp(auxxx->clavevacuna, vacunaaborrar)) {
				ant = auxxx;
				auxxx = auxxx->siguiente;
			}
			if (auxxx == inicioxx) {
				inicioxx = auxxx->siguiente;
				delete auxxx;
				auxxx = inicioxx;
			}
			else {
				ant->siguiente = auxxx->siguiente;
				delete auxxx;
				auxxx = inicioxx;
			}
			MessageBox(hwnd, "Vacuna eliminada", "AVISO", MB_OK | MB_ICONEXCLAMATION);
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MAIN_MENU), hwnd, MAINMENU);

		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK EDITARVACUNAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		auxxx = inicioxx;
		while (auxxx != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDD_EDITARVACUNAS_LISTA), LB_INSERTSTRING, 0, (LPARAM)auxxx->clavevacuna);
			auxxx = auxxx->siguiente;
		}
	}break;
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		menu(wParam, hwnd);
		switch (LOWORD(wParam)) {
		case IDD_EDITARVACUNAS_LISTA: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[20] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDD_EDITARVACUNAS_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDD_EDITARVACUNAS_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				auxxx = inicioxx;
				while (auxxx->siguiente != nullptr && strcmp(textolist, auxxx->clavevacuna) != 0) {
					auxxx = auxxx->siguiente;
				}
				SetDlgItemText(hwnd, IDD_EDITARVACUNAS_TIPO, auxxx->tipo);
				SetDlgItemText(hwnd, IDD_EDITARVACUNAS_MARCA, auxxx->marca);
				SetDlgItemText(hwnd, IDD_EDITARVACUNAS_CLAVEVACUNCA, auxxx->clavevacuna);
				SetDlgItemText(hwnd, IDD_EDITARVACUNAS_DESCRIPRCION, auxxx->descripcion);
				SetDlgItemInt(hwnd, IDD_EDITARVACUNAS_NODOSIS, (UINT)auxxx->nodosis, FALSE);
				SetDlgItemInt(hwnd, IDD_EDITARVACUNAS_PRECIO, (UINT)auxxx->precio, FALSE);

			}break;
			}

		}break;
		case IDD_EDITARVACUNAS_EDITARBTN: {
			GetDlgItemText(hwnd, IDD_EDITARVACUNAS_TIPO, auxxx->tipo, 15);
			GetDlgItemText(hwnd, IDD_EDITARVACUNAS_MARCA, auxxx->marca, 15);
			GetDlgItemText(hwnd, IDD_EDITARVACUNAS_CLAVEVACUNCA, auxxx->clavevacuna, 15);
			GetDlgItemText(hwnd, IDD_EDITARVACUNAS_DESCRIPRCION, auxxx->descripcion, 30);
			auxxx->nodosis = GetDlgItemInt(hwnd, IDD_EDITARVACUNAS_NODOSIS, NULL, FALSE);
			auxxx->precio = GetDlgItemInt(hwnd, IDD_EDITARVACUNAS_PRECIO, NULL, FALSE);

			MessageBox(hwnd, "Informacion modificada", "AVISO", MB_OK | MB_ICONEXCLAMATION);
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MAIN_MENU), hwnd, MAINMENU);

		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK LISTAVACUNAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		auxxx = inicioxx;
		while (auxxx != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDD_LISTVACUNAS_LISTA), LB_INSERTSTRING, 0, (LPARAM)auxxx->clavevacuna);
			auxxx = auxxx->siguiente;
		}
	}break;
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		menu(wParam, hwnd);
		switch (LOWORD(wParam)) {
		case IDD_LISTVACUNAS_LISTA: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[20] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDD_LISTVACUNAS_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDD_LISTVACUNAS_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				auxxx = inicioxx;
				while (auxxx->siguiente != nullptr && strcmp(textolist, auxxx->clavevacuna) != 0) {
					auxxx = auxxx->siguiente;
				}
				SetDlgItemText(hwnd, IDD_LISTVACUNAS_TIPO, auxxx->tipo);
				SetDlgItemText(hwnd, IDD_LISTVACUNAS_MARCA, auxxx->marca);
				SetDlgItemText(hwnd, IDD_LISTVACUNAS_CLAVEVACUNA, auxxx->clavevacuna);
				SetDlgItemText(hwnd, IDD_LISTVACUNAS_DESCRIPCION, auxxx->descripcion);
				SetDlgItemInt(hwnd, IDD_LISTVACUNAS_NODOSIS, (UINT)auxxx->nodosis, FALSE);
				SetDlgItemInt(hwnd, IDD_LISTVACUNAS_PRECIO, (UINT)auxxx->precio, FALSE);

			}break;
			}

		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK BORRARPERSONAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) { //idd dialog 1 
	switch (msg) {
	case WM_INITDIALOG: {
		aux = inicio;
		while (aux != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_BORRARPERSONA_LISTA), LB_INSERTSTRING, 0, (LPARAM)aux->nombres);
			aux = aux->siguiente;
		}
	}break;
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		menu(wParam, hwnd);
		switch (LOWORD(wParam)) {
		case IDC_BORRARPERSONA_LISTA: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[30] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_BORRARPERSONA_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_BORRARPERSONA_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				aux = inicio;
				while (aux->siguiente != nullptr && strcmp(textolist, aux->nombres) != 0) {
					aux = aux->siguiente;
				}
				strcpy_s(personaaborrar, textolist);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_APELLIDOPATERNO, aux->paterno);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_APELLIDOMATERNO, aux->materno);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_NOMBRES, aux->nombres);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_CURP, aux->curp);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_RFC, aux->rfc);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_DIA, aux->fechaCita.dia);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_MES, aux->fechaCita.mes);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_ANIO, aux->fechaCita.anio);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_CALLE, aux->calle);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_COLONIA, aux->colonia);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_CIUDAD, aux->ciudad);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_ESTADO, aux->estado);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_ESTADOCIVIL, aux->estadocivil);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_SEXO, aux->sexo);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_GRUPOOCUPACIONAL, aux->grupoocupa);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_PERFILRIESGO, aux->perfilriesgo);
				SetDlgItemText(hwnd, IDC_BORRARPERSONAS_ESTADOCIVIL, aux->estadocivil);
				SetDlgItemInt(hwnd, IDC_BORRARPERSONAS_PRETELEFONO, (UINT)aux->pretelefono, FALSE);
				SetDlgItemInt(hwnd, IDC_BORRARPERSONAS_TELEFONO, (UINT)aux->telefono, FALSE);
				bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 150, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_BORRARPERSONAS_BITMAP, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}break;
			}
		}break;
		case IDC_BORRARPERSONAS_BORRARBTN: {
			aux = inicio;
			RegistroPersonas* ant = nullptr;
			while (aux != nullptr && strcmp(aux->nombres, personaaborrar)) {
				ant = aux;
				aux = aux->siguiente;
			}
			if (aux == inicio) {
				inicio = aux->siguiente;
				delete aux;
				aux = inicio;
			}
			else {
				ant->siguiente = aux->siguiente;
				delete aux;
				aux = inicio;
			}
			cuantashay = cuantashay - 1;
			MessageBox(hwnd, "Persona eliminada", "AVISO", MB_OK | MB_ICONEXCLAMATION);
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MAIN_MENU), hwnd, MAINMENU);

		}break;
		}
	}

	}
	return FALSE;
}

BOOL CALLBACK EDITARPERSONAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		aux = inicio;
		while (aux != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDD_EDITARPERSONAS_LISTA), LB_INSERTSTRING, 0, (LPARAM)aux->nombres);
			aux = aux->siguiente;
		}
	}break;
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		menu(wParam, hwnd);
		switch (LOWORD(wParam)) {
		case IDD_EDITARPERSONAS_LISTA: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[30] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_BORRARPERSONA_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_BORRARPERSONA_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				aux = inicio;
				while (aux->siguiente != nullptr && strcmp(textolist, aux->nombres) != 0) {
					aux = aux->siguiente;
				}
				strcpy_s(personaaborrar, textolist);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_PATERNO, aux->paterno);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_MATERNO, aux->materno);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_NOMBRES, aux->nombres);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_CURP, aux->curp);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_RFC, aux->rfc);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_DIA, aux->fechaCita.dia);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_MES, aux->fechaCita.mes);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_anio, aux->fechaCita.anio);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_CALLE, aux->calle);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_COLONIA, aux->colonia);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_CIUDAD, aux->ciudad);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_ESTADO, aux->estado);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_ESTADOCIVIL, aux->estadocivil);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_SEXO, aux->sexo);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_GRUPOOCUPACIONAL, aux->grupoocupa);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_PERFILRIESGO, aux->perfilriesgo);
				SetDlgItemText(hwnd, IDD_EDITARPERSONAS_ESTADOCIVIL, aux->estadocivil);
				SetDlgItemInt(hwnd, IDD_EDITARPERSONAS_PRETELEFONO, (UINT)aux->pretelefono, FALSE);
				SetDlgItemInt(hwnd, IDD_EDITARPERSONAS_TELEFONO, (UINT)aux->telefono, FALSE);
				bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 150, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDD_EDITARPERSONAS_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}break;
			}
		}break;
		case IDD_EDITARPERSONAS_EDITARBTN: {
			SYSTEMTIME fechaCita;
			ZeroMemory(&fechaCita, sizeof(fechaCita));
			SendDlgItemMessage(hwnd, IDD_EDITARPERSONAS_DATEPICK, DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaCita);
			sprintf_s(aux->fechaCita.dia, "%d", fechaCita.wDay);
			sprintf_s(aux->fechaCita.mes, "%d", fechaCita.wMonth);
			sprintf_s(aux->fechaCita.anio, "%d", fechaCita.wYear);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_PATERNO, aux->paterno, 15);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_MATERNO, aux->materno, 15);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_NOMBRES, aux->nombres, 30);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_CURP, aux->curp, 19);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_RFC, aux->rfc, 14);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_SEXO, aux->sexo, 10);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_PERFILRIESGO, aux->perfilriesgo, 7);

			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_CALLE, aux->calle, 15);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_COLONIA, aux->colonia, 15);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_CIUDAD, aux->ciudad, 15);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_ESTADO, aux->estado, 15);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_ESTADOCIVIL, aux->estadocivil, 15);
			GetDlgItemText(hwnd, IDD_EDITARPERSONAS_GRUPOOCUPACIONAL, aux->grupoocupa, 15);
			aux->pretelefono = GetDlgItemInt(hwnd, IDD_EDITARPERSONAS_PRETELEFONO, NULL, FALSE);
			aux->telefono = GetDlgItemInt(hwnd, IDD_EDITARPERSONAS_TELEFONO, NULL, FALSE);
			strcpy_s(aux->foto, copiarfoto);
			MessageBox(hwnd, "Informacion modificada", "AVISO", MB_OK | MB_ICONEXCLAMATION);
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MAIN_MENU), hwnd, REGISTROPERSONAS);
		}break;
		case IDD_EDITARPERSONAS_MODIFICARIMAGENBTN: {
			OPENFILENAME ofn;

			char zFile[250]; //260 caracteres

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.lpstrFileTitle = copiarfoto;
			ofn.nMaxFileTitle = sizeof(copiarfoto);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;
			ofn.lpstrFileTitle = copiarfoto;

			if (GetOpenFileName(&ofn) == TRUE) {
				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 150, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDD_EDITARPERSONAS_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}
			else {
				MessageBox(hwnd, "No seleccionó una imagen", "AVISO", MB_OK | MB_ICONERROR);
			}
		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK LISTAPERSONAS(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		aux = inicio;
		while (aux != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LISTAP_LISTA), LB_INSERTSTRING, 0, (LPARAM)aux->nombres);
			aux = aux->siguiente;
		}
	}break;
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		menu(wParam, hwnd);
		switch (LOWORD(wParam)) {
		case IDC_LISTAP_CARNETBTN: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_CARNET), hwnd, CARNET);
		}break;
		case IDC_LISTAP_LISTA: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[30] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDC_LISTAP_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LISTAP_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				aux = inicio;
				while (aux->siguiente != nullptr && strcmp(textolist, aux->nombres) != 0) {
					aux = aux->siguiente;
				}
				strcpy_s(personaaborrar, textolist);
				SetDlgItemText(hwnd, IDC_LISTAP_APELLIDOPATERNO, aux->paterno);
				SetDlgItemText(hwnd, IDC_LISTAP_APELLIDOMATERNO, aux->materno);
				SetDlgItemText(hwnd, IDC_LISTAP_NOMBRES, aux->nombres);
				SetDlgItemText(hwnd, IDC_LISTAP_CURP, aux->curp);
				SetDlgItemText(hwnd, IDC_LISTAP_RFC, aux->rfc);
				SetDlgItemText(hwnd, IDC_LISTAP_DIA, aux->fechaCita.dia);
				SetDlgItemText(hwnd, IDC_LISTAP_MES, aux->fechaCita.mes);
				SetDlgItemText(hwnd, IDC_LISTAP_ANIO, aux->fechaCita.anio);
				SetDlgItemText(hwnd, IDC_LISTAP_CALLE, aux->calle);
				SetDlgItemText(hwnd, IDC_LISTAP_COLONIA, aux->colonia);
				SetDlgItemText(hwnd, IDC_LISTAP_CIUDADD, aux->ciudad);
				SetDlgItemText(hwnd, IDC_LISTAP_ESTADO, aux->estado);
				SetDlgItemText(hwnd, IDC_LISTAP_ESTADOCIVIL, aux->estadocivil);
				SetDlgItemText(hwnd, IDC_LISTAP_SEXO, aux->sexo);
				SetDlgItemText(hwnd, IDC_LISTAP_GRUPOOCUPACIONAL, aux->grupoocupa);
				SetDlgItemText(hwnd, IDC_LISTAP_PERFILRIESGO, aux->perfilriesgo);
				SetDlgItemText(hwnd, IDC_LISTAP_ESTADOCIVIL, aux->estadocivil);
				SetDlgItemInt(hwnd, IDC_LISTAP_PRETELEFONO, (UINT)aux->pretelefono, FALSE);
				SetDlgItemInt(hwnd, IDC_LISTAP_TELEFONO, (UINT)aux->telefono, FALSE);
				bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 150, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hwnd, IDC_LISTAP_BITMAP, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}break;
			}
		}break;
		
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK CARNET(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		auxxxx = inicioxxx;
		while (auxxxx != nullptr) {
			if (strcmp(aux->nombres, auxxxx->nombres) != 0) {
				auxxxx = auxxxx->siguiente;
			}
			else {
				SendMessage(GetDlgItem(hwnd, IDD_CARNET_LISTA), LB_INSERTSTRING, 0, (LPARAM)auxxxx->clavevacuna);
				auxxxx = auxxxx->siguiente;
			}
		}
		SetDlgItemText(hwnd, IDD_CARNET_APELLIDOPATERNO, aux->paterno);
		SetDlgItemText(hwnd, IDD_CARNET_APELLIDOMATERNO, aux->materno);
		SetDlgItemText(hwnd, IDD_CARNET_NOMBRES, aux->nombres);
		SetDlgItemText(hwnd, IDD_CARNET_CURP, aux->curp);
		SetDlgItemText(hwnd, IDD_CARNET_RFC, aux->rfc);
		SetDlgItemText(hwnd, IDD_CARNET_DIA, aux->fechaCita.dia);
		SetDlgItemText(hwnd, IDD_CARNET_MES, aux->fechaCita.mes);
		SetDlgItemText(hwnd, IDD_CARNET_ANIO, aux->fechaCita.anio);
		SetDlgItemText(hwnd, IDD_CARNET_CALLE, aux->calle);
		SetDlgItemText(hwnd, IDD_CARNET_COLONIA, aux->colonia);
		SetDlgItemText(hwnd, IDD_CARNET_CIUDAD, aux->ciudad);
		SetDlgItemText(hwnd, IDD_CARNET_ESTADO, aux->estado);
		SetDlgItemText(hwnd, IDD_CARNET_ESTADOCIVIL, aux->estadocivil);
		SetDlgItemText(hwnd, IDD_CARNET_SEXO, aux->sexo);
		SetDlgItemText(hwnd, IDD_CARNET_GRUPOOCUPACIONAL, aux->grupoocupa);
		SetDlgItemText(hwnd, IDD_CARNET_PERFILRIESGO, aux->perfilriesgo);
		SetDlgItemText(hwnd, IDD_CARNET_ESTADOCIVIL, aux->estadocivil);
		SetDlgItemInt(hwnd, IDD_CARNET_PRETELEFONO, (UINT)aux->pretelefono, FALSE);
		SetDlgItemInt(hwnd, IDD_CARNET_TELEFONO, (UINT)aux->telefono, FALSE);
		bmp = (HBITMAP)LoadImage(NULL, aux->foto, IMAGE_BITMAP, 150, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
		SendDlgItemMessage(hwnd, IDD_CARNET_BITMAP, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemInt(hwnd, IDD_CARNET_IDCARNET, (UINT)aux->carnetid, FALSE);

	}break;
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDD_CARNET_LISTA: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[20] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDD_CARNET_LISTA, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDD_CARNET_LISTA, LB_GETTEXT, indice, (LPARAM)textolist);
				auxxxx = inicioxxx;
				while (auxxxx->siguiente != nullptr && strcmp(textolist, auxxxx->clavevacuna) != 0) {
					auxxxx = auxxxx->siguiente;
				}
				SetDlgItemText(hwnd, IDD_CARNET_MARCA, auxxxx->vacuna.marca);
				SetDlgItemInt(hwnd, IDD_CARNET_LOTE, (UINT)auxxxx->lote, FALSE);
				SetDlgItemInt(hwnd, IDD_CARNET_DOSIS, (UINT)auxxxx->nodosis, FALSE);
				SetDlgItemText(hwnd, IDD_CARNET_DIAAPLICACION, auxxxx->fechaDosis.dia);
				SetDlgItemText(hwnd, IDD_CARNET_MESAPLICACION, auxxxx->fechaDosis.mes);
				SetDlgItemText(hwnd, IDD_CARNET_ANIOAPLICACION, auxxxx->fechaDosis.anio);
				SetDlgItemText(hwnd, IDD_CARNET_CENTRO, auxxxx->CentroVacuna);
				SetDlgItemText(hwnd, IDD_CARNET_DIASIG, auxxxx->fechaprox.dia);
				SetDlgItemText(hwnd, IDD_CARNET_MESSIG, auxxxx->fechaprox.mes);
				SetDlgItemText(hwnd, IDD_CARNET_ANIOSIG, auxxxx->fechaprox.anio);

			}break;
			}
		}break;
		case IDD_CARNET_REGRESARBTN: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MAIN_MENU), hwnd, MAINMENU);
		}break;
		case IDD_CARNET_AGREGARBTN: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_AGREGARCARNET), hwnd, AGREGARCARNET);
		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK AGREGARCARNET(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		auxxx = inicioxx;
		while (auxxx != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDD_AGREGARCARNET_LIST), LB_INSERTSTRING, 0, (LPARAM)auxxx->clavevacuna);
			auxxx = auxxx->siguiente;
		}
		SetDlgItemText(hwnd, IDD_AGREGARCARNET_NOMBRE, aux->nombres);
		SetDlgItemText(hwnd, IDD_AGREGARCARNET_CURP, aux->curp);
		SetDlgItemText(hwnd, IDD_AGREGARCARNET_PROXDIA, "00");
		SetDlgItemText(hwnd, IDD_AGREGARCARNET_PROXMES, "00");
		SetDlgItemText(hwnd, IDD_AGREGARCARNET_PROXANIO, "0000");
	}break;
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDD_AGREGARCARNET_LIST: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[20] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDD_AGREGARCARNET_LIST, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDD_AGREGARCARNET_LIST, LB_GETTEXT, indice, (LPARAM)textolist);
				auxxx = inicioxx;
				while (auxxx->siguiente != nullptr && strcmp(textolist, auxxx->clavevacuna) != 0) {
					auxxx = auxxx->siguiente;
				}

				SetDlgItemText(hwnd, IDD_AGREGARCARNET_CLAVE, auxxx->clavevacuna);
				SetDlgItemInt(hwnd, IDD_AGREGARCARNET_DOSIS, (UINT)auxxx->nodosis, FALSE);
				SetDlgItemText(hwnd, IDD_AGREGARCARNET_MARCAA, auxxx->marca);

			}break;
			}


		}break;
		case IDD_AGREGARCARNET_AGREGARBTN: {
			Carnet* nodouse = new Carnet;
			SYSTEMTIME fechaDosis;
			ZeroMemory(&fechaDosis, sizeof(fechaDosis));
			SendDlgItemMessage(hwnd, IDD_AGREGARCARNET_FECHA, DTM_GETSYSTEMTIME, 0, (LPARAM)&fechaDosis);
			sprintf_s(nodouse->fechaDosis.dia, "%d", fechaDosis.wDay);
			sprintf_s(nodouse->fechaDosis.mes, "%d", fechaDosis.wMonth);
			sprintf_s(nodouse->fechaDosis.anio, "%d", fechaDosis.wYear);

			GetDlgItemText(hwnd, IDD_AGREGARCARNET_NOMBRE, nodouse->nombres, 30);
			GetDlgItemText(hwnd, IDD_AGREGARCARNET_CURP, nodouse->curp, 19);
			GetDlgItemText(hwnd, IDD_AGREGARCARNET_MARCAA, nodouse->vacuna.marca, 15);
			GetDlgItemText(hwnd, IDD_AGREGARCARNET_CLAVE, nodouse->clavevacuna, 15);
			nodouse->nodosis = GetDlgItemInt(hwnd, IDD_AGREGARCARNET_DOSIS, NULL, FALSE);
			GetDlgItemText(hwnd, IDD_AGREGARCARNET_CENTRO, nodouse->CentroVacuna, 10);
			int azare = rand() % 1000 + 1;
			nodouse->lote = azare;

			GetDlgItemText(hwnd, IDD_AGREGARCARNET_PROXDIA, nodouse->fechaprox.dia, 5);
			GetDlgItemText(hwnd, IDD_AGREGARCARNET_PROXMES, nodouse->fechaprox.mes, 5);
			GetDlgItemText(hwnd, IDD_AGREGARCARNET_PROXANIO, nodouse->fechaprox.anio, 5);

			AgregarCarnet(nodouse);
			MessageBox(hwnd, "Vacuna agregada al carnet", "AVISO", MB_OK | MB_ICONEXCLAMATION);

			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_MAIN_MENU), hwnd, MAINMENU);
		}break;
		}

	}
	}
	return FALSE;
}

BOOL CALLBACK NUMEROCARNET(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;

	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		menu(wParam, hwnd);
		switch (LOWORD(wParam)) {
		case IDD_NUMEROCARNET_BUSCARBTN: {
			int buscar;
			buscar = GetDlgItemInt(hwnd, IDD_NUMEROCARNET_NUMCARNET, NULL, FALSE);
			Clave(wParam, hwnd, buscar);
			if (mostrar = true) {
				SetDlgItemInt(hwnd, IDD_NUMEROCARNET_IDCARNET, (UINT)buscar, FALSE);
				aux = inicio;
				while (aux != nullptr && (buscar != aux->carnetid)) {
					aux = aux->siguiente;
				}
				auxxxx = inicioxxx;
				while (auxxxx != nullptr) {
					if (strcmp(aux->nombres, auxxxx->nombres) != 0) {
						auxxxx = auxxxx->siguiente;
					}
					else {
						SendMessage(GetDlgItem(hwnd, IDD_NUMEROCARNET_LIST), LB_INSERTSTRING, 0, (LPARAM)auxxxx->clavevacuna);
						auxxxx = auxxxx->siguiente;
					}
				}
			}
			else {
				MessageBox(hwnd, "No se encontro", "AVISO", MB_OK | MB_ICONERROR);
			}

		}break;
		case IDD_NUMEROCARNET_LIST: {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char textolist[20] = { 0 };
				int indice = 0;
				indice = SendDlgItemMessage(hwnd, IDD_NUMEROCARNET_LIST, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDD_NUMEROCARNET_LIST, LB_GETTEXT, indice, (LPARAM)textolist);
				auxxxx = inicioxxx;
				while (auxxxx->siguiente != nullptr && strcmp(textolist, auxxxx->clavevacuna) != 0) {
					auxxxx = auxxxx->siguiente;
				}
				SetDlgItemText(hwnd, IDD_NUMEROCARNET_MARCA, auxxxx->vacuna.marca);
				SetDlgItemInt(hwnd, IDD_NUMEROCARNET_LOTE, (UINT)auxxxx->lote, FALSE);
				SetDlgItemInt(hwnd, IDD_NUMEROCARNET_NODOSIS, (UINT)auxxxx->nodosis, FALSE);
				SetDlgItemText(hwnd, IDD_NUMEROCARNET_APLDIA, auxxxx->fechaDosis.dia);
				SetDlgItemText(hwnd, IDD_NUMEROCARNET_APLMES, auxxxx->fechaDosis.mes);
				SetDlgItemText(hwnd, IDD_NUMEROCARNET_APLANIO, auxxxx->fechaDosis.anio);
				SetDlgItemText(hwnd, IDD_NUMEROCARNET_CENTRO, auxxxx->CentroVacuna);
				SetDlgItemText(hwnd, IDD_NUMEROCARNET_SIGDIA, auxxxx->fechaprox.dia);
				SetDlgItemText(hwnd, IDD_NUMEROCARNET_SIGMES, auxxxx->fechaprox.mes);
				SetDlgItemText(hwnd, IDD_NUMEROCARNET_SIGANIO, auxxxx->fechaprox.anio);

			}break;
			}
		}break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK MAINMENU(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_CLOSE: {
		escribirUsuarios();
		escribirPersonas();
		escribirVacunas();
		escribirCarnet();
		DestroyWindow(hwnd);
	}break;
	case WM_DESTROY: {
		PostQuitMessage(117);
	}break;
	case WM_COMMAND: {
		menu(wParam, hwnd);
		switch (LOWORD(wParam))
		{
		case IDC_MENU_REGP: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_REGISTROPERSONAS), hwnd, REGISTROPERSONAS);
		}break;
		case IDC_MENU_REGV: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_REGISTROVACUNAS), hwnd, REGISTROVACUNAS);
		}break;
		case IDC_MENU_MODP: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_EDITARPERSONAS), hwnd, EDITARPERSONAS);
		}break;
		case IDC_MENU_MODV: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_EDITARVACUNAS), hwnd, EDITARVACUNAS);
		}break;
		case IDC_MENU_DELETEP: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_BORRARPERSONAS), hwnd, BORRARPERSONAS);
		}break;
		case IDC_MENU_DELETEV: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_BAJADEVACUNAS), hwnd, BAJADEVACUNAS);
		}break;
		case IDC_MENU_REPORTENOM: {
			int contadorCARNET = contarNombres();
			RegistroPersonas* arreglo = new RegistroPersonas[contadorCARNET]; //mi puntero es igual a mi arreglo de carnet
			ArregloCarnet(arreglo, contadorCARNET);
			heapSort(arreglo, contadorCARNET);
			escribirTxtCarnet(wParam, hWnd, contadorCARNET, arreglo);
			delete[] arreglo;
			MessageBox(hWnd, "Reporte nombres  guardado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}break;
		case IDC_MENU_REPORTECAR: {
				int contadorNOMBRE = contarNombres();
				RegistroPersonas* arregloquick = new RegistroPersonas[contadorNOMBRE]; //mi puntero es igual a mi arreglo de carnet
				ArregloNombre(arregloquick, contadorNOMBRE);
				quickSort(arregloquick, 0, contadorNOMBRE - 1);
				escribirTxtNombre(wParam, hWnd, contadorNOMBRE, arregloquick);
				delete[] arregloquick;
				MessageBox(hWnd, "Reporte carnet guardado", "AVISO", MB_OK | MB_ICONINFORMATION);
			}break;
		case IDC_MENU_CARNET: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_NUMEROCARNET), hwnd, NUMEROCARNET);
		}break;
		case IDC_MENU_LISTAP: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_LISTAPERSONAS), hwnd, LISTAPERSONAS);
		}break;
		case IDC_MENU_LISTAV: {
			EndDialog(hwnd, 0);
			DialogBox(instGlobal, MAKEINTRESOURCE(IDD_LISTVACUNAS), hwnd, LISTAVACUNAS);
		}break;
		case IDC_MENU_SALIR: {
			escribirUsuarios();
			escribirPersonas();
			escribirVacunas();
			escribirCarnet();
			DestroyWindow(hwnd);
		}break;
		}
		{
		default:
			break;
		}
	}break;

	}
	return FALSE;
}


//Funciones WinAPi final code

//Funciones inicio code
 void AgregarPersona(RegistroPersonas* nuevo)
{
	if (inicio == nullptr)
	{
		inicio = new RegistroPersonas;
		aux = inicio;
		aux->anterior = nullptr;
		aux->siguiente = nullptr;
		strcpy_s(aux->paterno, nuevo->paterno);
		strcpy_s(aux->materno, nuevo->materno);
		strcpy_s(aux->nombres, nuevo->nombres);
		strcpy_s(aux->curp, nuevo->curp);
		strcpy_s(aux->rfc, nuevo->rfc);
		strcpy_s(aux->calle, nuevo->calle);
		strcpy_s(aux->colonia, nuevo->colonia);
		strcpy_s(aux->ciudad, nuevo->ciudad);
		strcpy_s(aux->estado, nuevo->estado);
		strcpy_s(aux->estadocivil, nuevo->estadocivil);
		strcpy_s(aux->foto, nuevo->foto);
		strcpy_s(aux->sexo, nuevo->sexo);
		strcpy_s(aux->grupoocupa, nuevo->grupoocupa);
		strcpy_s(aux->perfilriesgo, nuevo->perfilriesgo);

		strcpy_s(aux->fechaCita.dia, nuevo->fechaCita.dia);
		strcpy_s(aux->fechaCita.mes, nuevo->fechaCita.mes);
		strcpy_s(aux->fechaCita.anio, nuevo->fechaCita.anio);

		aux->pretelefono = nuevo->pretelefono;
		aux->telefono = nuevo->telefono;
		aux->carnetid = nuevo->carnetid;
	}
	else
	{
		aux = inicio;
		while (aux->siguiente != nullptr)
		{
			aux = aux->siguiente;
		}
		aux->siguiente = new RegistroPersonas;
		aux->siguiente->siguiente = nullptr;
		aux->siguiente->anterior = aux;
		aux = aux->siguiente;
		fin = aux;
		strcpy_s(aux->paterno, nuevo->paterno);
		strcpy_s(aux->materno, nuevo->materno);
		strcpy_s(aux->nombres, nuevo->nombres);
		strcpy_s(aux->curp, nuevo->curp);
		strcpy_s(aux->rfc, nuevo->rfc);
		strcpy_s(aux->calle, nuevo->calle);
		strcpy_s(aux->colonia, nuevo->colonia);
		strcpy_s(aux->ciudad, nuevo->ciudad);
		strcpy_s(aux->estado, nuevo->estado);
		strcpy_s(aux->estadocivil, nuevo->estadocivil);
		strcpy_s(aux->foto, nuevo->foto);
		strcpy_s(aux->sexo, nuevo->sexo);
		strcpy_s(aux->grupoocupa, nuevo->grupoocupa);
		strcpy_s(aux->perfilriesgo, nuevo->perfilriesgo);

		strcpy_s(aux->fechaCita.dia, nuevo->fechaCita.dia);
		strcpy_s(aux->fechaCita.mes, nuevo->fechaCita.mes);
		strcpy_s(aux->fechaCita.anio, nuevo->fechaCita.anio);

		aux->pretelefono = nuevo->pretelefono;
		aux->telefono = nuevo->telefono;
		aux->carnetid = nuevo->carnetid;
	}
}

void AgregarUsuario(user* nuevo)
{
	if (iniciox == nullptr)
	{
		iniciox = new user;
		auxx = iniciox;
		auxx->anterior = nullptr;
		auxx->siguiente = nullptr;
		strcpy_s(auxx->usuario, nuevo->usuario);
		strcpy_s(auxx->contra, nuevo->contra);
		strcpy_s(auxx->nombres, nuevo->nombres);
		strcpy_s(auxx->apmaterno, nuevo->apmaterno);
		strcpy_s(auxx->appaterno, nuevo->appaterno);
	}
	else
	{
		auxx = iniciox;
		while (auxx->siguiente != nullptr)
		{
			auxx = auxx->siguiente;
		}
		auxx->siguiente = new user;
		auxx->siguiente->siguiente = nullptr;
		auxx->siguiente->anterior = auxx;
		auxx = auxx->siguiente;
		finx = auxx;
		strcpy_s(auxx->usuario, nuevo->usuario);
		strcpy_s(auxx->contra, nuevo->contra);
		strcpy_s(auxx->nombres, nuevo->nombres);
		strcpy_s(auxx->apmaterno, nuevo->apmaterno);
		strcpy_s(auxx->appaterno, nuevo->appaterno);

	}
}

void AgregarVacuna(RegistroVacunas* nuevo)
{
	if (inicioxx == nullptr)
	{
		inicioxx = new RegistroVacunas;
		auxxx = inicioxx;
		auxxx->anterior = nullptr;
		auxxx->siguiente = nullptr;
		strcpy_s(auxxx->tipo, nuevo->tipo);
		strcpy_s(auxxx->marca, nuevo->marca);
		strcpy_s(auxxx->clavevacuna, nuevo->clavevacuna);
		strcpy_s(auxxx->descripcion, nuevo->descripcion);
		auxxx->nodosis = nuevo->nodosis;
		auxxx->precio = nuevo->precio;

	}
	else
	{
		auxxx = inicioxx;
		while (auxxx->siguiente != nullptr)
		{
			auxxx = auxxx->siguiente;
		}
		auxxx->siguiente = new RegistroVacunas;
		auxxx->siguiente->siguiente = nullptr;
		auxxx->siguiente->anterior = auxxx;
		auxxx = auxxx->siguiente;
		finxx = auxxx;
		strcpy_s(auxxx->tipo, nuevo->tipo);
		strcpy_s(auxxx->marca, nuevo->marca);
		strcpy_s(auxxx->clavevacuna, nuevo->clavevacuna);
		strcpy_s(auxxx->descripcion, nuevo->descripcion);
		auxxx->nodosis = nuevo->nodosis;
		auxxx->precio = nuevo->precio;
	}
}

void escribirUsuarios()
{
	auxx = iniciox;
	ofstream escribirUsuario;
	escribirUsuario.open("Usuarios.bin", ios::out | ios::binary | ios::trunc);
	if (escribirUsuario.is_open())
	{
		while (auxx != nullptr)
		{
			escribirUsuario.write((char*)auxx, sizeof(user));
			auxx = auxx->siguiente;
		}
		escribirUsuario.close();
		MessageBox(hWnd, "Usuarios guardado", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(hWnd, "No se pudo abrir el archivo", "AVISO", MB_OK | MB_ICONERROR);
	}
}

void leerUsuarios()
{
	ifstream leerUsuarios;
	leerUsuarios.open("Usuarios.bin", ios::in | ios::binary);

	auxx = iniciox;
	if (leerUsuarios.is_open())
	{
		user* userleido = new user;

		while (!leerUsuarios.read((char*)userleido, sizeof(user)).eof())
		{

			while (auxx != nullptr && auxx->siguiente != nullptr)
			{
				auxx = auxx->siguiente;
			}
			if (auxx == nullptr)
			{
				iniciox = userleido;
				iniciox->siguiente = nullptr;
				iniciox->anterior = nullptr;
				auxx = iniciox;
			}
			else
			{
				auxx->siguiente = userleido;
				auxx->siguiente->anterior = auxx;
				auxx = auxx->siguiente;
				auxx->siguiente = nullptr;
			}

			userleido = new user;

		}
		leerUsuarios.close();
	}
}

void escribirPersonas()
{
	aux = inicio;
	ofstream escribirPersona;
	escribirPersona.open("Personas.bin", ios::out | ios::binary | ios::trunc);
	if (escribirPersona.is_open())
	{
		while (aux != nullptr)
		{
			escribirPersona.write((char*)aux, sizeof(RegistroPersonas));
			aux = aux->siguiente;
		}
		escribirPersona.close();
		MessageBox(hWnd, "Personas guardado", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else
	{
		MessageBox(hWnd, "No se pudo abrir el archivo", "AVISO", MB_OK | MB_ICONERROR);
	}
}

void leerPersonas()
{
	ifstream leerPersonas;
	leerPersonas.open("Personas.bin", ios::in | ios::binary);

	aux = inicio;
	if (leerPersonas.is_open())
	{
		RegistroPersonas* personaleido = new RegistroPersonas;

		while (!leerPersonas.read((char*)personaleido, sizeof(RegistroPersonas)).eof())
		{

			while (aux != nullptr && aux->siguiente != nullptr)
			{
				aux = aux->siguiente;
			}
			if (aux == nullptr)
			{
				inicio = personaleido;
				inicio->siguiente = nullptr;
				inicio->anterior = nullptr;
				aux = inicio;
			}
			else
			{
				aux->siguiente = personaleido;
				aux->siguiente->anterior = aux;
				aux = aux->siguiente;
				aux->siguiente = nullptr;
			}

			personaleido = new RegistroPersonas;
			cuantashay++;
		}
		leerPersonas.close();
	}
}

void escribirVacunas() {
	auxxx = inicioxx;
	ofstream escribirVacuna;
	escribirVacuna.open("Vacunas.bin", ios::out | ios::binary | ios::trunc);
	if (escribirVacuna.is_open()) {
		while (auxxx != nullptr) {
			escribirVacuna.write((char*)auxxx, sizeof(RegistroVacunas));
			auxxx = auxxx->siguiente;
		}
		escribirVacuna.close();
		MessageBox(hWnd, "Vacunas guardado", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		MessageBox(hWnd, "No se pudo abrir el archivo", "AVISO", MB_OK | MB_ICONERROR);
	}
}

void leerVacunas()
{
	ifstream leerVacunas;
	leerVacunas.open("Vacunas.bin", ios::in | ios::binary);

	auxxx = inicioxx;
	if (leerVacunas.is_open()) {
		RegistroVacunas* vacunaleido = new RegistroVacunas;

		while (!leerVacunas.read((char*)vacunaleido, sizeof(RegistroVacunas)).eof()) {

			while (auxxx != nullptr && auxxx->siguiente != nullptr) {
				auxxx = auxxx->siguiente;
			}
			if (auxxx == nullptr) {
				inicioxx = vacunaleido;
				inicioxx->siguiente = nullptr;
				inicioxx->anterior = nullptr;
				auxxx = inicioxx;
			}
			else {
				auxxx->siguiente = vacunaleido;
				auxxx->siguiente->anterior = auxxx;
				auxxx = auxxx->siguiente;
				auxxx->siguiente = nullptr;
			}

			vacunaleido = new RegistroVacunas;

		}
		leerVacunas.close();
	}
}

void BuscarUsuario(char usuario[50]) {
	nousuariosregistrados = 1;
	usuarionoencontrado = 1;
	auxx = iniciox;
	if (auxx == nullptr) {
		nousuariosregistrados = 0;
	}
	else {
		while (auxx != nullptr && strcmp(auxx->usuario, usuario) != 0) {
			auxx = auxx->siguiente;
		}
		if (auxx == nullptr) {
			usuarionoencontrado = 0;
		}
	}
}

void BuscarContrasena(char usuario[50]) {
	contrasenaincorrecta = 1;
	auxx = iniciox;
	if (auxx == nullptr) {
		nousuariosregistrados = 0;
	}
	else {
		while (auxx != nullptr && strcmp(auxx->contra, usuario) != 0) {
			auxx = auxx->siguiente;
		}
		if (auxx == nullptr) {
			contrasenaincorrecta = 0;
		}
	}
}

void BuscarUsuarioRepetido(char usuario[50]) {
	usuariorepetido = 0;
	auxx = iniciox;
	if (iniciox == nullptr) {
		usuariorepetido = 0;
	}
	while (auxx != nullptr && strcmp(auxx->usuario, usuario) != 0) {
		auxx = auxx->siguiente;
	}
	if (auxx == nullptr) {
		usuariorepetido = 0;
	}
	else {
		usuariorepetido = 1;
	}
}

void BuscarClaveVacunaRepetida(char usuario[15]) {
	clavevacunarepetido = 0;
	auxxx = inicioxx;
	if (inicioxx == nullptr) {
		clavevacunarepetido = 0;
	}
	while (auxxx != nullptr && strcmp(auxxx->clavevacuna, usuario) != 0) {
		auxxx = auxxx->siguiente;
	}
	if (auxxx == nullptr) {
		clavevacunarepetido = 0;
	}
	else {
		clavevacunarepetido = 1;
	}
}

int BusquedaBinaria(int izquierda, int derecha, int buscado) {

	if (derecha >= izquierda) {
		int mitad = izquierda + (derecha - izquierda) / 2;
		int i = 0;
		aux = inicio;
		while (i != mitad) {//solo va a moverse por la lista la cantidad de veces que sea el valor "mitad"
			aux = aux->siguiente;
			i++;
		}
		if (aux->carnetid == buscado) {
			u = aux;
			return mitad;
		}
		if (aux->carnetid > buscado) {
			return BusquedaBinaria(izquierda, mitad - 1, buscado);
		}
		else {
			return BusquedaBinaria(mitad + 1, derecha, buscado);
		}
	}
	return -1;
}

void escribirCarnet() {
	auxxxx = inicioxxx;
	ofstream escribirCarnet;
	escribirCarnet.open("Carnet.bin", ios::out | ios::binary | ios::trunc);
	if (escribirCarnet.is_open()) {
		while (auxxxx != nullptr) {
			escribirCarnet.write((char*)auxxxx, sizeof(Carnet));
			auxxxx = auxxxx->siguiente;
		}
		escribirCarnet.close();
		MessageBox(hWnd, "Archivo guardado", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		MessageBox(hWnd, "No se pudo abrir el archivo", "AVISO", MB_OK | MB_ICONERROR);
	}
}

void leerCarnet() {
	ifstream leerCarnet;
	leerCarnet.open("Carnet.bin", ios::in | ios::binary);

	auxxxx = inicioxxx;
	if (leerCarnet.is_open()) {
		Carnet* carnetleido = new Carnet;

		while (!leerCarnet.read((char*)carnetleido, sizeof(Carnet)).eof()) {

			while (auxxxx != nullptr && auxxxx->siguiente != nullptr) {
				auxxxx = auxxxx->siguiente;
			}
			if (auxxxx == nullptr) {
				inicioxxx = carnetleido;
				inicioxxx->siguiente = nullptr;
				inicioxxx->anterior = nullptr;
				auxxxx = inicioxxx;
			}
			else {
				auxxxx->siguiente = carnetleido;
				auxxxx->siguiente->anterior = auxxxx;
				auxxxx = auxxxx->siguiente;
				auxxxx->siguiente = nullptr;
			}

			carnetleido = new Carnet;

		}
		leerCarnet.close();
	}
}

void AgregarCarnet(Carnet* nuevo) {
	if (inicioxxx == nullptr) {
		inicioxxx = new Carnet;
		auxxxx = inicioxxx;
		auxxxx->anterior = nullptr;
		auxxxx->siguiente = nullptr;
		strcpy_s(auxxxx->curp, nuevo->curp);
		strcpy_s(auxxxx->clavevacuna, nuevo->clavevacuna);
		strcpy_s(auxxxx->CentroVacuna, nuevo->CentroVacuna);
		auxxxx->nodosis = nuevo->nodosis;
		auxxxx->lote = nuevo->lote;
		strcpy_s(auxxxx->fechaDosis.dia, nuevo->fechaDosis.dia);
		strcpy_s(auxxxx->fechaDosis.mes, nuevo->fechaDosis.mes);
		strcpy_s(auxxxx->fechaDosis.anio, nuevo->fechaDosis.anio);
		strcpy_s(auxxxx->nombres, nuevo->nombres);
		strcpy_s(auxxxx->vacuna.marca, nuevo->vacuna.marca);
		strcpy_s(auxxxx->fechaprox.dia, nuevo->fechaprox.dia);
		strcpy_s(auxxxx->fechaprox.mes, nuevo->fechaprox.mes);
		strcpy_s(auxxxx->fechaprox.anio, nuevo->fechaprox.anio);

	}
	else {
		auxxxx = inicioxxx;
		while (auxxxx->siguiente != nullptr) {
			auxxxx = auxxxx->siguiente;
		}
		auxxxx->siguiente = new Carnet;
		auxxxx->siguiente->siguiente = nullptr;
		auxxxx->siguiente->anterior = auxxxx;
		auxxxx = auxxxx->siguiente;
		finxxx = auxxxx;
		strcpy_s(auxxxx->curp, nuevo->curp);
		strcpy_s(auxxxx->clavevacuna, nuevo->clavevacuna);
		strcpy_s(auxxxx->CentroVacuna, nuevo->CentroVacuna);
		auxxxx->nodosis = nuevo->nodosis;
		auxxxx->lote = nuevo->lote;
		strcpy_s(auxxxx->fechaDosis.dia, nuevo->fechaDosis.dia);
		strcpy_s(auxxxx->fechaDosis.mes, nuevo->fechaDosis.mes);
		strcpy_s(auxxxx->fechaDosis.anio, nuevo->fechaDosis.anio);
		strcpy_s(auxxxx->nombres, nuevo->nombres);
		strcpy_s(auxxxx->vacuna.marca, nuevo->vacuna.marca);
		strcpy_s(auxxxx->fechaprox.dia, nuevo->fechaprox.dia);
		strcpy_s(auxxxx->fechaprox.mes, nuevo->fechaprox.mes);
		strcpy_s(auxxxx->fechaprox.anio, nuevo->fechaprox.anio);

	}
}

void Clave(WPARAM wParam, HWND hwnd, int buscado) {

	int tamaño = 0;
	aux = inicio;
	while (aux != nullptr) {
		aux = aux->siguiente;
		tamaño++;
	}

	//regresar resultado
	int derecha = tamaño;
	int respuesta = BusquedaBinaria(0, derecha, buscado);
	if (respuesta == -1) {
		mostrar = false;
	}
	else {
		MessageBox(hWnd, "Encontrado", "AVISO", MB_OK | MB_ICONWARNING);
		mostrar = true;
	}

}

int contarNombres() {
	aux = inicio;
	int acum = 0;
	while (aux != NULL) {
		acum++;
		aux = aux->siguiente;
	}
	return acum;
}

void ArregloNombre(RegistroPersonas R[], int tamaño) {
	aux = inicio;
	for (int i = 0; i < tamaño; i++) {
		R[i] = *aux;
		aux = aux->siguiente;
	}
}

void quickSort(RegistroPersonas R[], int left, int right) {
	int i = left, j = right;

	RegistroPersonas tmp;

	char pivote[150];

	strcpy_s(pivote, R[(left + right) / 2].nombres);

	while (i <= j)
	{
		while (strcmp(R[i].nombres, pivote) < 0)
			i++;
		while (strcmp(R[j].nombres, pivote) > 0)
			j--;

		if (i <= j) {
			tmp = R[i];
			R[i] = R[j];
			R[j] = tmp;
			i++;
			j--;
		}
	}

	if (left < j) //si no entran a ninguno de los ifs ya esta ordenado
		quickSort(R, left, j);
	if (i < right)
		quickSort(R, i, right);
}

void ArregloCarnet(RegistroPersonas R[], int tamaño) {
	aux = inicio; // se iguala al inicio de la lista

	for (int i = 0; i < tamaño; i++) // copiar y pegar de la lista de carnet
	{
		R[i] = *aux;
		aux = aux->siguiente;
	}
}

void OrdenarCarnet(RegistroPersonas R[], int may, int men)
{
	RegistroPersonas temporal;
	int MAYOR = men; //igual a la raiz
	int izq = 2 * men + 1; //el hijo de la izquierda
	int dere = 2 * men + 2;//el otro hijo

	if (izq < may && (R[izq].carnetid > R[MAYOR].carnetid)) //
	{
		MAYOR = izq; //almacena el indice     
		//si el valor izq tiene un valor mayor al de raiz
	}

	if (dere < may && (R[dere].carnetid > R[MAYOR].carnetid))
	{
		MAYOR = dere;
		//lo mismo pero para el otro lado 
	}

	if (MAYOR != men) //INTERCAMBIAR LUGARES
	{
		temporal = R[men];
		R[men] = R[MAYOR];
		R[MAYOR] = temporal;

		OrdenarCarnet(R, may, MAYOR); //funcion recursiva, para acomodar el arbolito
	}

}

void heapSort(RegistroPersonas R[], int may)
{
	RegistroPersonas temporal;

	for (int i = may / 2 - 1; i >= 0; i--)
	{
		OrdenarCarnet(R, may, i);
	}

	for (int i = may - 1; i >= 0; i--)
	{

		temporal = R[0];
		R[0] = R[i];
		R[i] = temporal;

		OrdenarCarnet(R, i, 0);
	}
}

void escribirTxtNombre(WPARAM wParam, HWND hWnd, int contadorNOMBRE, RegistroPersonas* arreglo) {
	ofstream archivo;
	archivo.open("ReporteNombres.txt");


	for (int i = 0; i < contadorNOMBRE; i++)
	{
		/*string nuevotel = to_string(arreglo[i].Telefono);*/
		archivo
			<< "No. de Carnet: " << arreglo[i].carnetid << endl
			<< "Nombre: " << arreglo[i].nombres << endl
			<< "Apellido Paterno: " << arreglo[i].paterno << endl << "Apellido Materno:" << ' ' << arreglo[i].materno << endl
			<< "CURP: " << arreglo[i].curp << endl
			<< "RFC: " << arreglo[i].rfc << endl
			<< "Estado: " << arreglo[i].estado << endl << "Ciudad:" << ' ' << arreglo[i].ciudad << endl << "Colonia:" << ' ' << arreglo[i].colonia << endl << "Calle:" << ' ' << arreglo[i].calle << endl
			<< "Estado Civil: " << arreglo[i].estadocivil << endl
			<< "Grupo Ocupacional: " << arreglo[i].grupoocupa << endl
			<< "Perfil de riesgo: " << arreglo[i].perfilriesgo << endl
			<< "Teléfono: " << arreglo[i].pretelefono << arreglo[i].telefono << endl << endl;
	}


	archivo.close();
}

void escribirTxtCarnet(WPARAM wParam, HWND hWnd, int contadorNOMBRE, RegistroPersonas* arreglo) {
	ofstream archivo;
	archivo.open("ReporteCarnet.txt");


	for (int i = 0; i < contadorNOMBRE; i++)
	{
		/*string nuevotel = to_string(arreglo[i].Telefono);*/
		archivo
			<< "No. de Carnet: " << arreglo[i].carnetid << endl
			<< "Nombre: " << arreglo[i].nombres << endl
			<< "Apellido Paterno: " << arreglo[i].paterno << endl << "Apellido Materno:" << ' ' << arreglo[i].materno << endl
			<< "CURP: " << arreglo[i].curp << endl
			<< "RFC: " << arreglo[i].rfc << endl
			<< "Estado: " << arreglo[i].estado << endl << "Ciudad:" << ' ' << arreglo[i].ciudad << endl << "Colonia:" << ' ' << arreglo[i].colonia << endl << "Calle:" << ' ' << arreglo[i].calle << endl
			<< "Estado Civil: " << arreglo[i].estadocivil << endl
			<< "Grupo Ocupacional: " << arreglo[i].grupoocupa << endl
			<< "Perfil de riesgo: " << arreglo[i].perfilriesgo << endl
			<< "Teléfono: " << arreglo[i].pretelefono << arreglo[i].telefono << endl << endl << "test";
	}


	archivo.close();
}

void BuscarCurpRepetido(char usuario[19]) {
	curprepetido = 0;
	aux = inicio;
	if (inicio == nullptr) {
		curprepetido = 0;
	}
	while (aux != nullptr && strcmp(aux->curp, usuario) != 0) {
		aux = aux->siguiente;
	}
	if (aux == nullptr) {
		curprepetido = 0;
	}
	else {
		curprepetido = 1;
	}
}

void BuscarRfcRepedito(char usuario[19]) {
	rfcrepetido = 0;
	aux = inicio;
	if (inicio == nullptr) {
		rfcrepetido = 0;
	}
	while (aux != nullptr && strcmp(aux->rfc, usuario) != 0) {
		aux = aux->siguiente;
	}
	if (aux == nullptr) {
		rfcrepetido = 0;
	}
	else {
		rfcrepetido = 1;
	}
}


//void secuencial(char nombres[30]) {
//	auxxxx = inicioxxx;
//	while (auxxxx != nullptr && strcmp(auxxxx->nombres, nombres) != 0) {
//		auxxxx = auxxxx->siguiente;
//	}
//	if (auxx == nullptr) {
//		return secuencial;
//	}
//}
//




//Funciones final code



