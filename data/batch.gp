do for [fn in system("dir /b")] {
    len=strlen(fn)
    form=substr(fn,len-3,len)
    name=substr(fn,0,len-4)

    if (form eq ".txt") {
        set term push
        load "plotStyle.gp"
        # set term pdf
        set term pdfcairo enhanced crop

        data=sprintf("%s",fn)
        filename=sprintf("./pics/%s.pdf",name)

        set out filename

        unset border
        r(x) = (x <= 0 ? 1 : sqrt(x))
        g(x) = (x <= 0 ? 1: x**3)
        b(x) = (x <= 0 ? 1 : sin(2*pi*x))
        set palette functions r(gray),g(gray),b(gray)

        plot data matrix with image
        set o
        set term pop
    }
}