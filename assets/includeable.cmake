#from: https://stackoverflow.com/questions/410980/include-a-text-file-in-a-c-program-as-a-char
function(make_includeable input_file output_file) 
    file(READ ${input_file} content)
    set(delim "for_C++_include")
    set(content "R\"${delim}(${content})${delim}\"")
    file(WRITE ${output_file} "${content}")
endfunction(make_includeable)