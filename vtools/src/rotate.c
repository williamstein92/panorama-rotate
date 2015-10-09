///////////////////////////////////////////////////////////////////////////////
//
#include <math.h>
#include <string.h>
// for testing
#include <stdio.h>
#include <stdlib.h>
//
#include "vector.h"
#include "rotate.h"
//
///////////////////////////////////////////////////////////////////////////////

//// rotate ///////////////////////////////////////////////////////////////////
//
// rotate an img by given pitch, roll, yaw
// return error
//
uint32_t *
rotate(float roll,
       float pitch,
       float yaw,
       uint32_t *in_img,
       uint32_t *out_img)
{
    //// init /////////////////////////////////////////////////////////////////
    //
    float lat, lng, nx, ny, nz, clt, slt, r, pi, tau;
    uint32_t xx, yy, x, y, z, ilt, iln, h, w;
    struct vector v, vr, vp, vy, ar, ap, ay, pix;
    //
    pi = 4.0*atan(1.0);
    tau = pi*2;
    //
    ///////////////////////////////////////////////////////////////////////////

    //// size config //////////////////////////////////////////////////////////
    //
    // image height
    h = 1792;
    // image width
    w = 3584;
    // radius of sphere
    r = 570.41131604135256;
    //
    ///////////////////////////////////////////////////////////////////////////

    //// angles to single vector //////////////////////////////////////////////
    // see vector.c
    //
    // roll
    vector_set(&ar, 1, 0, 0, roll);
    vector_from_axis_angle(&vr, &ar);
    //
    // pitch
    vector_set(&ap, 0, 1, 0, pitch);
    vector_from_axis_angle(&vp, &ap);
    //
    // yaw
    // if (yaw > 180) {
    //     yaw -= 360;
    // }
    //
    vector_set(&ay, 0, 0, 1, yaw);
    vector_from_axis_angle(&vy, &ay);
    //
    // combine quaternions into v
    vector_multiply(&vp, &vy);
    vector_multiply(&vr, &vp);
    //
    vector_clone(&vr, &v);
    // vector_normalize(&v);
    //
    ///////////////////////////////////////////////////////////////////////////

    //// rotation /////////////////////////////////////////////////////////////
    //
    for (y = 0; y < h; y++) {
        yy = y * w * 3;

        // y to sphere latitude
        lat = (float) y / h * pi;

        // store the values
        slt = sin(lat);
        clt = cos(lat);

        for (x = 0; x < w; x++) {
            xx = x * 3;

            // x to sphere longitude
            lng = (float) x / w * tau - pi;

            //// spherical to cartesian ///////////////////////////////////////
            //
            nx = r * slt * cos(lng);
            ny = r * slt * sin(lng);
            nz = r * clt;
            //
            ///////////////////////////////////////////////////////////////////

            //// rotate ///////////////////////////////////////////////////////
            // see vector.c
            //
            // translate pixel into vector
            vector_set(&pix, nx, ny, nz, 0);
            // and rotate the pixel
            vector_conjugate(&pix, &v);
            // vector_print(&pix);
            //
            ///////////////////////////////////////////////////////////////////

            //// x,y,z -> lat,lng /////////////////////////////////////////////
            
            lat = (float) acos(pix.z / r) / pi * h;
            lng = (float) (atan2(pix.y, pix.x) + pi) / tau * w;
            //
            // pixel coords must be int
            ilt = (int) lat;
            iln = (int) lng;
            //
            // wrap latitude
            if (ilt >= h) {
                ilt -= h;
            }
            //
            // wrap longitude
            if (iln >= w) {
                iln -= w;
            }
            //
            ///////////////////////////////////////////////////////////////////

            for (z = 0; z < 3; z++) {
                int pos = ilt * w * 3 + iln * 3 + z;

                // pos > h * w * 3 + w * 3 + 3
                if (pos > 19278339) {
                    printf("ERROR OVERFLOW");
                    return out_img;
                }

                out_img[yy + xx + z] = in_img[pos];
            }
        }
    }
    //
    ///////////////////////////////////////////////////////////////////////////

    // we did it, we did it
    return out_img;
}
//
///////////////////////////////////////////////////////////////////////////////

//// test /////////////////////////////////////////////////////////////////////
//
// void
// test_rotate(void)
// {
//     uint32_t *in_img, *out_img;
//     int i, j, k;

//     in_img = malloc(sizeof(uint32_t *) * 1792);
//     out_img = in_img;

//     for (i = 0; i < 1792; i++) {
//         in_img[i] = malloc(sizeof(uint32_t *) * 3584);
//         for (j = 0; j < 3584; j++) {
//             in_img[i][j] = malloc(sizeof(uint32_t) * 3);
//             for (k = 0; k < 3; k++) {
//                 in_img[i][j][k] = 0;
//             }
//         }
//     }

//     rotate(45, 45, 45, in_img, out_img);
// }
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
int
main(void)
{
    // test_rotate();
    return 0;
}
//
///////////////////////////////////////////////////////////////////////////////
