do for [fn in system("dir /b")] {
    len=strlen(fn)
    form=substr(fn,len-3,len)
    name=substr(fn,0,len-4)

    if (form eq ".txt") {
        set term push
        load "plotStyle.gp"
        # set term pdf
        set term pdfcairo crop
        set size square 1,1

        data=sprintf("%s",fn)
        filename=sprintf("%s.pdf",name)

        set out filename
        plot data matrix with image
        set o
        set term pop
    }
}