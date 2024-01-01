/**
 * Example use of GHTTP (Embedded library for parsing HTTP requests and creating responses) - Part of example ghttp_ethercard_post_file_upload
 * 
 * Dave.C, 2020
 */

#if !defined(ROUTEROOT_H)
#define ROUTEROOT_H

namespace examples { namespace ghttp_ethercard_post_file_upload {

/**
 * An example implemented route for HTTP GET requests covering route "/"
 */
class RouteRoot : public ghttp::Route<Shared> {
public:
    bool Handler(Shared &s) {
        if (s.request.url!=T("/")) {
            // Not handled
            return false;
        }
        
        bool checksum_done = false;
        uint16_t checksum = 0;
        
        // Has user post form content?  and does it contain our file?
        if (s.request.post_file_variable[T("my_file")]) {                                // Like PHP isset($_FILES["my_file_data"])
            auto *fn = s.request.post_file_variable[T("my_file")]->filename.Get();        // File content like $_FILES["my_file"].name
            auto *mt = s.request.post_file_variable[T("my_file")]->mimetype.Get();        // File content like $_FILES["my_file"].type
            auto fs = s.request.post_file_variable[T("my_file")]->filesize;                // File content like $_FILES["my_file"].size
            auto *f = s.request.post_file_variable[T("my_file")]->value.Get();            // In PHP this would be taken into file system by move_uploaded_file(...) but here you have access to raw data from request decode

            // Size will be zero and filename will be empty string if no file selected
            if (fs && fn && strlen(fn)) {
                // In reality the request buffer is going to be limited in size so likely use of this feature will be configuration uploads or perhaps f/w updates either as a diff or
                // a few seperate files selected by user, saved to a larger medium like SD card and then re-assembled into full binary for flashing.  Now you could use client 
                // side JS with AJAX and post chunks to your device so there are options
                Serial.print("file: '");
                Serial.print(fn);
                Serial.print("'. type: '");
                Serial.print(mt);
                Serial.print("'. size: ");
                Serial.println(fs, DEC);

                // Checksum file content
                for(uint16_t i=0; i<fs; i++) {
                    checksum += 255 ^ f[i];
                }
                checksum ^= 0xffffu;
                checksum_done = true;
            }
        }
        
        // Set response buffer
        s.response.SetBuffer(s.request.GetBuffer(), 0, 0); // Cannot use request anymore as no offset which is fine for this route

        // Set status OK.  We are doing this because response instance is reused so may have last status of unauthorised
        s.response.status.Set(ghttp::HTTPResponseStatus::STATUS_OK);
        s.response.Header(T("HTTP/1.1"));

        s.response.HeaderComplete();
        // Output response data + Complete
        s.response+=T("<html><body>");
        if (checksum_done) {
            // Output checksum of uploaded file
            s.response+=T("<p style='color:blue'><strong>File checksum: ");
            s.response.AppendUInt<uint16_t>(checksum);
            s.response+=T("</strong></p>");
        }
        // Our multipart form which is required for file upload
        s.response+=T("<form method='post' action='/' enctype='multipart/form-data'>");
        s.response+=T("<label for='file_upload'>Choose a text or data file to checksum:</label>");
        s.response+=T("<input type='file' id='file_upload' name='my_file' accept='text/plain, .dat' />");
        s.response+=T("<input type='submit' name='submit' value='Submit' />");        
        s.response+=T("</form>");
        s.response+=T("<p style='color:red'>NOTES: Only small files are accepted due to buffering on device, select a test file from example folder.  If you type another route you should see a forbidden message</p>");
        s.response+=T("</body></html>");
        s.response.Complete();
        
        return true; // We have handled
    }
}; // class RouteRoot

} } // namespace examples::ghttp_ethercard_post_file_upload

#endif // !defined(ROUTEROOT_H)
