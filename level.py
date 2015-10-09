#==== imports =================================================================
#
import os
from math import *
import subprocess
import scipy.misc
#
import config
import vtools
#
#______________________________________________________________________________

#==== level image =============================================================
# returns roll, pitch, yaw, execution time
#
def level(in_file, out_file, roll=None, pitch=None, yaw=None):

    #==== get accelerometer data ==============================================
    #
    angles   = {'-ricohroll': roll, '-ricohpitch': pitch, '-compass': yaw}
    # path to input file
    path    = os.path.join(os.getcwd(), in_file)

    for switch, angle in angles.items():
        #==== run exif tool ===================================================
        #
        if angle is None:
            cmd = config.exif_cmd + [switch, path]
            angle = subprocess.check_output(cmd)
        #
        #______________________________________________________________________

        if switch is '-ricohroll':
            roll = float(angle) if angle is not '' else 0.0
        elif switch is '-ricohpitch':
            pitch = float(angle) if angle is not '' else 0.0
        elif switch is '-compass':
            yaw = float(angle) if angle is not '' else 0.0
    #
    #__________________________________________________________________________

    #==== rotate image ========================================================
    #
    # open image
    in_img = scipy.misc.imread(in_file)
    # rotate
    out_img = vtools.rotate(roll, pitch, yaw, in_img)
    # save image
    scipy.misc.imsave(out_file, out_img)
    #
    #__________________________________________________________________________

    return roll, pitch, yaw
#
#______________________________________________________________________________
