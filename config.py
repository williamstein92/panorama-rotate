#==== imports =================================================================
#
import os
#
#______________________________________________________________________________

#==== config ==================================================================
#
base = os.path.dirname(os.path.abspath(__file__))

exif_cmd = [os.path.join(base, 'exif', 'exiftool'),  "-s3"]
#
#______________________________________________________________________________
