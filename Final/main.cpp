#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <gdiplus.h>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

std::atomic<bool> exitCondition(false);

int keyPressed(int key){
    return (GetAsyncKeyState(key) & 0x8000 != 0);
}
    using namespace Gdiplus;
    using namespace std;

    struct Coordinates {
        int x;
        int y;
    };

    bool SaveHBITMAPToJPEG(HBITMAP hBitmap, const wchar_t* filename);
    int GetEncoderClsid(const wchar_t* format, CLSID* pClsid);
    // 🔴🔴🔴 what are these deffinitions
    // 🔴🔴🔴 are the declarations or library calls?



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




std::mutex globalMutex;
int screenshotCounter = 0;


int screenshotIntervalMs = 250; // 4 screenshots per second (1000 ms / 4)

int buffer_size = 200; // make this 1000 later

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





void loop1() {

    int recieved_screenWidth = GetSystemMetrics(SM_CXSCREEN); // Get the screen width
    int recieved_screenHeight = GetSystemMetrics(SM_CYSCREEN); // Get the screen height
    while (!exitCondition) {


        // Check the contents of the text file
        std::ifstream filestat("status.txt");
        std::string content;
        filestat >> content;

        if (content == "yes") {
            exitCondition = true;
        }




        int localscreenshotCounter;
        {
            std::lock_guard<std::mutex> lock(globalMutex);
            // Access and read the shared variable
            screenshotCounter++;
            screenshotCounter = screenshotCounter % buffer_size;

            {
                std::ofstream fout("Counter_location\\counter.txt");
                fout<<screenshotCounter;
                fout.close();
            }

            localscreenshotCounter = screenshotCounter;

        }
        // Other code for the first loop
        // 🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩


            std::string pathreset = "Keyboard/";
            std::string clickfilename = "click_" + std::to_string(localscreenshotCounter) + ".txt";
            std::ofstream fileclick(pathreset + clickfilename);

            if(fileclick.is_open())
                {
                    fileclick << "0";
                }

            fileclick.close();

            // Capture the screen


            // =============================================================================
            // This is the chunk for image extraction
            // 🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡
            // =============================================================================

            HDC hdcScreen = GetDC(NULL);
            int screenWidth = 1920;
            int screenHeight = 1080;
            HDC hdcMem = CreateCompatibleDC(hdcScreen);
            HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
            HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);
            BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);


            // 🔴🔴🔴 here im saving the file with a jpeg extension but is it actually just a bitmap image that im renaming and not remorphing?
            // 🔴🔴🔴 thing is that maybe it might be wasting a lot of space because of this

            // Generate a unique filename for each screenshot
            std::wstringstream wss;
            wss << L"Screenshots\\scrsh_" << localscreenshotCounter << L".jpeg"; // Changed file extension to .jpeg
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
            // 🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡
            // =============================================================================

            // =============================================================================
            // This is the chunk for metadata extraction
            // 🟠🟠🟠🟠🟠🟠🟠🟠🟠🟠
            // =============================================================================

            // // Create an array of strings
                // std::vector<std::string> stringArray = {"apple", "banana", "cherry", "date" };
                // const int size = 4;
                // {
                // ofstream fout("Counter_location\\temp.txt");
                // if(fout.is_open())
                //     {
                //     for(int i = 0; i <size; i++)
                //     {
                //         fout << stringArray[i];
                //         fout << "\n";
                //     }
                //     fout << "=============================\n";
                //     }
                // fout.close();
                // }

            // Get cursor location

            Coordinates cursorCoordinates = GetCursorLocation();

            int new_x_coord = (cursorCoordinates.x * screenWidth)/recieved_screenWidth;
            int new_y_coord = (cursorCoordinates.y * screenHeight)/recieved_screenHeight;

            if (cursorCoordinates.x != -1 && cursorCoordinates.y != -1) {
                std::cout << "Cursor Location: X = " << new_x_coord << ", Y = " << new_y_coord << std::endl;
            } else {
                std::cout << "Failed to get cursor location." << std::endl;
            }


            // 🔵🔵🔵 Working window extraction code!
            HWND hwnd = GetForegroundWindow();
            char windowTitle[256];

            if (hwnd != NULL) {
                GetWindowText(hwnd, windowTitle, sizeof(windowTitle));
                std::cout << "Active Window Title: " << windowTitle << std::endl;
            } else {
                std::cerr << "Failed to get active window." << std::endl;
            }

            std::string path = "Meta/";
            std::string metafilename = "metadata_" + std::to_string(localscreenshotCounter) + ".txt";
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

                    file << "Active Windows:";
                    file << "\n";
                    file << windowTitle ;

                }

            file.close();




            // =============================================================================


            // Sleep for the specified interval
            Sleep(screenshotIntervalMs);














        // 🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩🟩
    }
}

void loop2() {
    while (!exitCondition) {
        int localscreenshotCounter2;
        int localscreenshotCounter3;
        {
            std::lock_guard<std::mutex> lock(globalMutex);
            // Access and read the shared variable
            localscreenshotCounter2 = screenshotCounter;
        }

        if(keyPressed(VK_LBUTTON)){
            std::cout << "🟩🟩🟩🟩🟩🟩🟩 Click has been detected." << std::endl;
            localscreenshotCounter3 = localscreenshotCounter2 - 1;
            localscreenshotCounter3 = localscreenshotCounter3 % buffer_size;


            std::string pathreset = "Keyboard/";
            std::string clickfilename = "click_" + std::to_string(localscreenshotCounter3) + ".txt";
            std::ofstream fileclick(pathreset + clickfilename);

            if(fileclick.is_open())
                {
                    fileclick << "1";
                }

            fileclick.close();

        }
        // Other code for the second loop
        // Sleep(125);
    }
}

int main() {

    // 🔴🔴🔴 why do i need this?
    // Get the path to the executable
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);


    // 🔴🔴🔴 is this basically extracting the current pwd?
    // Extract the directory path
    std::wstring directory = exePath;
    size_t lastBackslash = directory.find_last_of(L"\\");
    if (lastBackslash != std::wstring::npos) {
        directory = directory.substr(0, lastBackslash + 1);
    }



    // 🔴🔴🔴 What is the GDI+ library?
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    // std::cout << recieved_screenWidth << std::endl;
    // std::cout << recieved_screenHeight << std::endl;
    // 🟣🟣🟣 Turns out the metrics that it is seeing is 1280*720



    std::thread t1(loop1);
    std::thread t2(loop2);

    t1.join();
    t2.join();

    std::ofstream outFilestat("status.txt");
    outFilestat << "ready";
    outFilestat.close();

    // Shutdown GDI+
    GdiplusShutdown(gdiplusToken);
    return 0;
}


