// import libraries

// initiate main function

// setup the buffer_size🔵

// setup intra_extraction_time_period🔵

// initiate counter_no as a global variable so that all threads can access and read its current value
// counter_no =0🔵

// initiate thread 1 (takes care of all non [ keyboard inputs ] and write to buffer)

    // initiate thread 2 (takes care of all keyboard inputs and write to buffer)
        

        // initiate external for loop (infinite loop)

            // local_counter = copy of global counter
            
            // initiate inner for loop (infinite loop)
                // if local_counter == global_counter

                    // extract keyboard input data
                    // append to file keyboard_input_local_counter_no.txt
                    
                // else
                    // break loop




    // initiate the infinite for loop.🔵


        // counter_no++ (modify globally)🔵

        // counter_no = counterno modulo (buffer_size)🔵
            // (essentially just overwrites all the previous data in the buffer)

        //  save this counter_no to a file so it can be read later 🔵
            // remember that when u use the counter no to read from the buffer, u need to delete 1 from it becaue i may or may not have that data. i can afford to loose that data

        // screenshot current state of window 🔵
        // save as img_counter_no.png🔵

        // create metadata_counter_no.txt🔵
        
        // write time stamp to metadata_counter_no.txt

        // extract total active windows and write to metadata_counter_no.txt

        // extract current active window and write to metadata_counter_no.txt

        // extract mouse location and write to metadata_counter_no.txt🔵

        // extract eye location and write to metadata_counter_no.txt

        // extract input cursor location and write to metadata_counter_no.txt


        // pause process for time(intra_extraction_time_period)
