message(STATUS "Building documentation")
find_program(MakeExecutable make
  DOC "The make executable"
  REQUIRED
  )
message(STATUS "Building documentation")
find_program(Rst2PdfExecutable rst2pdf
  DOC "The rst2pdf executable"
  REQUIRED
  )
