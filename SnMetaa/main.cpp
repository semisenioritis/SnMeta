    #include <windows.h>
    #include <iostream>
    #include <string>
    #include <sstream>
    #include <fstream>
    #include <gdiplus.h>
    #include <vector>


    using namespace Gdiplus;
    using namespace std;

    struct Coordinates {
        int x;
        int y;
    };

    bool SaveHBITMAPToJPEG(HBITMAP hBitmap, const wchar_t* filename);
    int GetEncoderClsid(const wchar_t* format, CLSID* pClsid);






    Coordinates GetCursorLocation() {
        POINT cursorPos;
        if (GetCursorPos(&cursorPos)) {
            Coordinates coordinates;
            coordinates.x = cursorPos.x;
            coordinates.y = cursorPos.y;
            return coordinates;
        }

        // Return default values if unable to get cursor position
        Coordinates defaultCoordinates = { -1, -1 };
        return defaultCoordinates;
    }





    // Function to list all open windows
    BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
        if (IsWindowVisible(hwnd)) {
            char windowTitle[256];
            GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
            if (strlen(windowTitle) > 0) {
                cout << "Window Title: " << windowTitle << endl;
                vector<HWND>* windows = reinterpret_cast<vector<HWND>*>(lParam);
                windows->push_back(hwnd);
            }
        }
        return TRUE;
    }

    void ListOpenWindows() {
        vector<HWND> windows;
        EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windows));
    }

    // Function to get the active window
    void GetActiveWindowInfo() {
        HWND activeWindow = GetForegroundWindow();
        char windowTitle[256];
        GetWindowTextA(activeWindow, windowTitle, sizeof(windowTitle));
        cout << "Active Window: " << windowTitle << endl;
    }

    int main() {
        int screenshotIntervalMs = 250; // 4 screenshots per second (1000 ms / 4)
        int screenshotCounter = 0;
        int buffer_size = 10; // make this 1000 later

        // Get the path to the executable
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(NULL, exePath, MAX_PATH);

        // Extract the directory path
        std::wstring directory = exePath;
        size_t lastBackslash = directory.find_last_of(L"\\");
        if (lastBackslash != std::wstring::npos) {
            directory = directory.substr(0, lastBackslash + 1);
        }

        // Construct the full path to the "currloc" directory
        std::wstring currloc = directory + L"Counter_location";

        // Delete the directory if it exists
        if (RemoveDirectoryW(currloc.c_str())) {

        }

        // Create a new "Screenshots" directory next to the executable
        if (!CreateDirectoryW(currloc.c_str(), NULL)) {
            std::wcerr << L"Failed to create the currloc directory." << std::endl;
            return 1;
        }


        // Construct the full path to the "Screenshots" directory
        std::wstring screenshotsDir = directory + L"Screenshots";

        // Delete the directory if it exists
        if (RemoveDirectoryW(screenshotsDir.c_str())) {

        }

        // Create a new "Screenshots" directory next to the executable
        if (!CreateDirectoryW(screenshotsDir.c_str(), NULL)) {
            std::wcerr << L"Failed to create the scr directory." << std::endl;
            return 1;
        }



        // Construct the full path to the "Meta" directory
        std::wstring metaDir = directory + L"Meta";

        // Delete the directory if it exists
        if (RemoveDirectoryW(metaDir.c_str())) {

        }

        // Create a new "Screenshots" directory next to the executable
        if (!CreateDirectoryW(metaDir.c_str(), NULL)) {
            std::wcerr << L"Failed to create the meta directory." << std::endl;
            return 1;
        }


        // Construct the full path to the "keyip" directory
        std::wstring keyipDir = directory + L"Keyboard";

        // Delete the directory if it exists
        if (RemoveDirectoryW(keyipDir.c_str())) {

        }

        // Create a new "Screenshots" directory next to the executable
        if (!CreateDirectoryW(keyipDir.c_str(), NULL)) {
            std::wcerr << L"Failed to create the keyip directory." << std::endl;
            return 1;
        }







        // Initialize GDI+
        GdiplusStartupInput gdiplusStartupInput;
        ULONG_PTR gdiplusToken;
        GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

        int recieved_screenWidth = GetSystemMetrics(SM_CXSCREEN); // Get the screen width
        int recieved_screenHeight = GetSystemMetrics(SM_CYSCREEN); // Get the screen height

        while (true) {
            // Capture the screen

            screenshotCounter++;
            screenshotCounter = screenshotCounter % buffer_size;



            {
            ofstream fout("Counter_location\\counter.txt");
            fout<<screenshotCounter<<endl;
            }


            // =============================================================================
            // This is the chunk for image extraction
            // 🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴
            // =============================================================================

            HDC hdcScreen = GetDC(NULL);
            int screenWidth = 1920;
            int screenHeight = 1080;
            HDC hdcMem = CreateCompatibleDC(hdcScreen);
            HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
            BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);

            // Generate a unique filename for each screenshot
            std::wstringstream wss;
            wss << L"Screenshots\\scrsh_" << screenshotCounter << L".jpeg"; // Changed file extension to .jpeg
            std::wstring filename = wss.str();

            // Save the screenshot as a JPEG file
            if (SaveHBITMAPToJPEG(hBitmap, filename.c_str())) {
                std::wcout << L"Screenshot captured and saved as " << filename << std::endl;

            } else {
                std::wcerr << L"Failed to save the screenshot." << std::endl;
            }

            // Clean up resources
            SelectObject(hdcMem, hOldBitmap);
            DeleteObject(hBitmap);
            DeleteDC(hdcMem);
            ReleaseDC(NULL, hdcScreen);

            // =============================================================================
            // 🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴🔴
            // =============================================================================

            // =============================================================================
            // This is the chunk for metadata extraction
            // 🟠🟠🟠🟠🟠🟠🟠🟠🟠🟠
            // =============================================================================

            // Create an array of strings
            std::vector<std::string> stringArray = {"apple", "banana", "cherry", "date" };
            const int size = 4;
            {
            ofstream fout("Counter_location\\temp.txt");

            if(fout.is_open())
                {
                for(int i = 0; i <size; i++)
                {
                    fout << stringArray[i];
                    fout << "\n";
                }
                fout << "=============================\n";
                }

            fout.close();
            }

            // Get cursor location

            Coordinates cursorCoordinates = GetCursorLocation();

            int new_x_coord = (cursorCoordinates.x * screenWidth)/recieved_screenWidth;
            int new_y_coord = (cursorCoordinates.y * screenHeight)/recieved_screenHeight;

            if (cursorCoordinates.x != -1 && cursorCoordinates.y != -1) {
                std::cout << "Cursor Location: X = " << new_x_coord << ", Y = " << new_y_coord << std::endl;
            } else {
                std::cout << "Failed to get cursor location." << std::endl;
            }

            // // Construct a different filename for the second file
            // wss.str(L""); // Clear the stringstream
            // wss << L"Screenshots\\newfile-" << screenshotCounter << L".txt"; // Use a different name
            // std::wstring newFilename = wss.str();

            // // Open the second file for writing in append mode using wfstream
            // std::wfstream newFout;
            // newFout.open(newFilename.c_str(), std::ios::out | std::ios::app); // Open in append mode


            std::string path = "Meta/";
            std::string metafilename = "metadata_" + std::to_string(screenshotCounter) + ".txt";
            std::ofstream file(path + metafilename);



            if(file.is_open())
                {

                {
                    file << new_x_coord;
                    file << "\n";
                }
                {
                    file << new_y_coord;
                    file << "\n";
                }
                file << "=_=_=_=_=_=_=_=_=_=\n";
                }

            file.close();



            // // List open windows
            // ListOpenWindows();

            // // Get the active window
            // GetActiveWindowInfo();




            // =============================================================================


            // Sleep for the specified interval
            Sleep(screenshotIntervalMs);
        }

        // Shutdown GDI+
        GdiplusShutdown(gdiplusToken);

        return 0;
    }

    bool SaveHBITMAPToJPEG(HBITMAP hBitmap, const wchar_t* filename) {
        Gdiplus::Bitmap bitmap(hBitmap, NULL);

        CLSID clsid;
        if (GetEncoderClsid(L"image/jpeg", &clsid) == -1) { // Changed format to JPEG
            return false;
        }

        EncoderParameters encoderParams;
        encoderParams.Count = 1;
        encoderParams.Parameter[0].Guid = EncoderQuality;
        encoderParams.Parameter[0].Type = EncoderParameterValueTypeLong;
        encoderParams.Parameter[0].NumberOfValues = 1;
        ULONG quality = 95; // Adjust JPEG quality here (0-100)
        encoderParams.Parameter[0].Value = &quality;

        return bitmap.Save(filename, &clsid, &encoderParams) == Gdiplus::Ok;
    }

    int GetEncoderClsid(const wchar_t* format, CLSID* pClsid) {
        UINT num = 0;           // Number of image encoders
        UINT size = 0;          // Size of the image encoder array in bytes

        Gdiplus::GetImageEncodersSize(&num, &size);
        if (size == 0) {
            return -1;  // Failure
        }

        Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
        if (pImageCodecInfo == NULL) {
            return -1;  // Failure
        }

        Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

        for (UINT j = 0; j < num; ++j) {
            if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
                *pClsid = pImageCodecInfo[j].Clsid;
                free(pImageCodecInfo);
                return j;  // Success
            }
        }

        free(pImageCodecInfo);
        return -1;  // Failure
    }
