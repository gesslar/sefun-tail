// This sefun is not compatible with old-style efun::tail() as 1) it accepts
// a second argument of the number of lines, and 2) it returns a string rather
// than printing out the results. 
varargs string tail(string file, int num_lines)
{
    string *lines = ({ }) ;
    int fsize ;
    string ret = "" ;
    int chunk_size = 1000 ;
    int done ;

    seteuid( geteuid( previous_object() ) );

    if( nullp( file ) ) error("Missing arg 1 to tail()\n") ;
    if( nullp( num_lines  ) ) num_lines = 25 ;
    
    if( num_lines > 100 ) num_lines = 100 ;

    fsize = file_size( file ) ;
    if( fsize < 1 ) return "" ;

    // If the file's size is less than or equal the chunk size
    // no need to iterate, just send back the file contents
    if( fsize <= chunk_size ) return read_file( file ) ;

    while( !done )
    {
        string *temp_lines ;
        int temp_size ;
        int new_size ;
        string *chunk_lines ;
        string chunk ;
        int chop ;
        int start ; // this value is static and will retain its new value over each loop

        if( nullp( start ) ) start = fsize - chunk_size ;
        if( start < 0 )
        {
            chunk_size = chunk_size + start ;
            start = 0 ;
        }

        chunk = read_bytes( file, start, chunk_size ) ;
        chop = strsrch( chunk, "\n" ) ;

        // move the pointer forward to after the first \n
        start = start + chop + 1;
        // modify the chunk to after the first \n
        chunk = chunk[ chop+1 .. ] ;

        chunk_lines = explode( chunk, "\n" ) ;
        temp_lines = chunk_lines + lines  ;
        temp_size = sizeof( temp_lines ) ;

        if( temp_size >= num_lines )
        {
            lines = temp_lines[ < num_lines.. ] ;
            done = 1 ;
        }
        else
        {
            lines = temp_lines ;
            start -= chunk_size ;
        }
    }

    return implode( lines, "\n" ) + "\n" ;
}
