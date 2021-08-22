#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char *procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("CS210_Starter_PY_Code");
	PyErr_Print();	
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen: 
		You sent me: Test
Return:
	100 is returned to the C++	
*/
int callIntFunc(string proc, string param)
{
	char *procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char *paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject *pName, *pModule, *pDict, *pFunc, *pValue = nullptr, *presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"CS210_Starter_PY_Code");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char *procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject *pName, *pModule, *pDict, *pFunc, *pValue = nullptr, *presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"CS210_Starter_PY_Code");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}

void displayMenu() {
	cout << "1: Produce a list of all items purchased in a given day along with the number of times each item was purchased." << endl;
	cout << "2: Produce a number representing how many times a specific item was purchased in a given day." << endl;
	cout << "3: Produce a text-based histogram listing all items purchased in a given day, along with a representation of the number of times each item was purchased." << endl;
	cout << "4: Exit." << endl;
	cout << "Enter your selection as a number 1, 2, 3, or 4." << endl;
}

/// <summary>
/// Returns character i amount of times (multiplier).
/// </summary>
/// <param name="character">The character to return.</param>
/// <param name="multiplier">How many times to return the character.</param>
string charByMultiplier(char character, int multiplier) {
	string val;
	for (int i = 0; i < multiplier; i++) {
		val += character;
	}

	return val;
}

void produceHistogram() {
	// write items and quantities to 'frequency.dat'
	CallProcedure("writeItemsSold");

	ifstream inFS;      // Input file stream
	string fileData;    // data read from the file
	string item;        // temporary item data value from file
	int qty = 0;		// temporary quantity data value from file
	map<string, int> dict;

	// Try to open file
	inFS.open("frequency.dat");
	if (!inFS.is_open()) {
		cout << "Could not open file frequency.dat." << endl;
		throw std::invalid_argument("could not open file frequency.dat");
	}

	// move to first line
	inFS >> fileData;

	while (!inFS.fail()) {
		stringstream ss(fileData);
		getline(ss, item, ':'); // get first line, item, separated by delimiter ":"
		ss >> qty; // get quantity, after delimeter.

		dict[item] = qty; // add those to dict/map
		inFS >> fileData; // continue reading next line
	}

	if (!inFS.eof()) {
		cout << "Input failure before reaching end of file." << endl;
	}

	// if item declared, fill it into the dict/map .. key/value with quantity.
	if (!item.empty()) {
		dict[item] = qty;
	}

	inFS.close(); // Done with file, so close it

	// iterate through dict/map
	for (auto const& x : dict)
	{
		cout << x.first  // string (key), the item
			<< ':'
			<< charByMultiplier('*', x.second) // string's value, the quantity
			<< endl;
	}
}

void main()
{
	displayMenu();
	string input;
	cin >> input;

	while (true) {
		// try/catch because if anything but an int is inputted, stoi() throws an exception.
		try
		{
			// Produce a list of all items purchased in a given day along with the number of times each item was purchased.
			if (stoi(input) == 1) {
				system("cls");

				CallProcedure("displayTimesItemPurchased");

				// waits for keypress "confirmation" that user wants to continue
				system("pause");

				// returns to menu ("catch" block) for further input
				throw std::invalid_argument("finished");
			}
			// Produce a number representing how many times a specific item was purchased in a given day.
			else if (stoi(input) == 2) {
				system("cls");

				cout << "Enter the item you would like to search for: ";
				string item;
				cin >> item;

				int count = callIntFunc("displayTimesItemPurchased", item);
				cout << "Sales for that item: " << count << endl;

				// waits for keypress "confirmation" that user wants to continue
				system("pause");

				// returns to menu ("catch" block) for further input
				throw std::invalid_argument("finished");
			}
			// Produce a text-based histogram listing all items purchased in a given day, along with a representation of the number of times each item was purchased.
			else if (stoi(input) == 3) {
				system("cls");

				produceHistogram();

				// waits for keypress "confirmation" that user wants to continue
				system("pause");

				// returns to menu ("catch" block) for further input
				throw std::invalid_argument("finished");
			}
			// Exit.
			else if (stoi(input) == 4) {
				break;
			}
			else {
				// invalid value (not an option).
				throw std::invalid_argument("received invalid value");
			}
		}
		catch (const std::exception& e)
		{
			// clear screen and display menu again
			system("cls");
			displayMenu();

			// ignore last input, get new input
			cin.ignore();
			cin >> input;
		}
	}
}