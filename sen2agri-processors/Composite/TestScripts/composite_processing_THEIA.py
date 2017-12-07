#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
_____________________________________________________________________________

   Program:      Sen2Agri-Processors
   Language:     Python
   Copyright:    2015-2016, CS Romania, office@c-s.ro
   See COPYRIGHT file for details.
   
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
_____________________________________________________________________________

"""
from __future__ import print_function

import os
import shutil
import glob
import argparse
import csv
from sys import argv
import datetime
import subprocess
import pipes
import time
import os.path
import xml.etree.ElementTree as ET
from xml.dom import minidom

def runCmd(cmdArray):
    start = time.time()
    print(" ".join(map(pipes.quote, cmdArray)))
    res = subprocess.call(cmdArray)
    print("OTB app finished in: {}".format(datetime.timedelta(seconds=(time.time() - start))))
    if res != 0:
        print("OTB application error")
        exit(1)
    return res

parser = argparse.ArgumentParser(description='Composite Python processor for THEIA S2 data')

parser.add_argument('--applocation', help='The path where the sen2agri is built', default="", required=False)
parser.add_argument('--syntdate', help='L3A synthesis date', required=True)
parser.add_argument('--synthalf', help='Half synthesis', required=True)
parser.add_argument('--input', help='The list of products xml descriptors', required=True, nargs='+')
parser.add_argument('--res', help='The requested resolution in meters', required=True)
parser.add_argument('--outdir', help="Output directory", required=True)
parser.add_argument('--wdir', help="Working directory", required=False)
parser.add_argument('--bandsmap', help="Bands mapping file location", required=True)

USE_COMPRESSION=False
REMOVE_TEMP=False

args = parser.parse_args()

syntDate = args.syntdate
syntHalf = args.synthalf

inputList = args.input

resolution = args.res
bandsMap = args.bandsmap
appLocation = args.applocation
outDir = args.outdir
if args.inputdir:
    wDir = args.wdir
else:
    wDir=outDir

#defintion for filenames
outMasks = os.join(wDir, 'masksCLD_WAT_SNW.tif')
outImgBands =os.join(wDir, 'res' + resolution + '.tif')
outCld = os.join(wDir,'cld' + resolution + '.tif')
outWat = os.join(wDir + 'wat' + resolution + '.tif')
outSnow = os.join(wDir,'snow' + resolution + '.tif')
outAot = os.join(wDir, 'aot' + resolution + '.tif')
outEdg = os.join(wDir, 'edg' + resolution + '.tif')
outWeightAotFile = os.join(wDir, 'WeightAot#.tif')
outWeightCloudFile = os.join(wDir, 'WeightCloud#.tif')
outTotalWeightFile = os.join(wDir, 'WeightTotal#.tif')
outL3AFile = os.join(outDir, 'L3AResult#_' + resolution + 'M.tif')

WEIGHT_AOT_MIN="0.33"
WEIGHT_AOT_MAX="1"
AOT_MAX="0.8"

COARSE_RES="240"
SIGMA_SMALL_CLD="2"
SIGMA_LARGE_CLD="10"

WEIGHT_DATE_MIN="0.5"

outWeights = os.join(outDir, 'L3AResult#_weights.tif')
outRefls = os.join(outDir, 'L3AResult#_refls.tif')
outFlags = os.join(outDir, 'L3AResult#_flags.tif')

if os.path.exists(outDir):
    if not os.path.isdir(outDir):
        print("Can't create the output directory because there is a file with the same name")
        print("Remove: " + outDir)
        exit(1)
else:
    os.makedirs(outDir)

i = 0

print("Processing started: " + str(datetime.datetime.now()))
start = time.time()
for xml in inputList:

    runCmd(["otbcli", "MaskHandler",
            appLocation,
            "-xml", xml,
            "-out", outSpotMasks,
            "-sentinelres", resolution])

    counterString = str(i)
    mod=outL3AFile.replace("#", counterString)
    out_w=outWeights.replace("#", counterString)
    out_r=outRefls.replace("#", counterString)
    out_f=outFlags.replace("#", counterString)
    
    out_w_Aot=outWeightAotFile.replace("#", counterString)
    out_w_Cloud=outWeightCloudFile.replace("#", counterString)
    out_w_Total=outTotalWeightFile.replace("#", counterString)
   
    runCmd(["otbcli", "CompositePreprocessing2",
            appLocation,
            "-xml", xml,
            "-res", resolution,
            "-msk", outMasks,
            "-outres", outImgBands,
            "-outcmres", outCld,
            "-outwmres", outWat,
            "-outsmres", outSnow,
            "-outaotres", outAot,
            "-outedgres", outEdg])

    runCmd(["otbcli", "WeightAOT",
            appLocation,
            "-xml", xml,
            "-in", outAot,
            "-waotmin", WEIGHT_AOT_MIN,
            "-waotmax", WEIGHT_AOT_MAX,
            "-aotmax", AOT_MAX,
            "-out", out_w_Aot])

    runCmd(["otbcli", "WeightOnClouds",
            appLocation,
            "-inxml", xml,
            "-incldmsk", outCld,
            "-coarseres", COARSE_RES,
            "-sigmasmallcld", SIGMA_SMALL_CLD,
            "-sigmalargecld", SIGMA_LARGE_CLD,
            "-out", out_w_Cloud])

    runCmd(["otbcli", "TotalWeight",
            appLocation,
            "-xml", xml,
            "-waotfile", out_w_Aot,
            "-wcldfile", out_w_Cloud,
            "-l3adate", syntDate,
            "-halfsynthesis", syntHalf,
            "-wdatemin", WEIGHT_DATE_MIN,
            "-out", out_w_Total])
    if (i==0):
        runCmd(["otbcli", "UpdateSynthesisS2",
                appLocation,
                "--cl2arefl", outImgBands,
                "-cl2acsm", outCld,
                "-cl2awm", outWat,
                "-cl2asm", outSnow,
                "-cl2aw", out_w_Total,
                "-cl2aedg", None,
                "-bluebandidx", "0",
                "-cl3arefl", out_r,
                "-cl3aflag", out_f,
                "-cl3aw", out_w])
    else:
        tmpOut_w = out_w
        tmpOut_r = out_r
        tmpOut_f = out_f
        runCmd(["otbcli", "UpdateSynthesisS2",
                appLocation,
                "--cl2arefl", outImgBands,
                "-cl2acsm", outCld,
                "-cl2awm", outWat,
                "-cl2asm", outSnow,
                "-cl2aw", out_w_Total,
                "-cl2aedg", None,
                "-bluebandidx", "0",
                "-cl3arefl", out_r,
                "-cl3aflag", out_f,
                "-cl3aw", out_w,
                "-pl3aw", tmpOut_w,
                "-pl3arefl", tmpOut_f,
                "-pl3aflag", tmpOut_r])

    if USE_COMPRESSION:
        tmpOut_w += '?gdal:co:COMPRESS=DEFLATE'
        tmpOut_d += '?gdal:co:COMPRESS=DEFLATE'
        tmpOut_r += '?gdal:co:COMPRESS=DEFLATE'
        tmpOut_f += '?gdal:co:COMPRESS=DEFLATE'
        tmpOut_rgb += '?gdal:co:COMPRESS=DEFLATE'

    runCmd(["otbcli", "CompositeSplitter2",
            appLocation,
            "-in", mod,
            "-xml", xml,
            "-bmap", bandsMap,
            "-outweights", tmpOut_w,
            "-outdates", tmpOut_d,
            "-outrefls", tmpOut_r,
            "-outflags", tmpOut_f,
            "-outrgb", tmpOut_rgb])

    prevL3A = ["-prevl3aw", out_w, "-prevl3ad", out_d, "-prevl3ar", out_r, "-prevl3af", out_f]

    i += 1

if i == 0:
    print("No L2A products found !")
    exit(1)

print("Processing finished: " + str(datetime.datetime.now()))
print("Total execution time: {}".format(datetime.timedelta(seconds=(time.time() - start))))

'''
''' and None
