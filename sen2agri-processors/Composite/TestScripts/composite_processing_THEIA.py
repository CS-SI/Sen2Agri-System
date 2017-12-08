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
import string
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

if __name__ == '__main__':
        
    parser = argparse.ArgumentParser(description='Composite Python processor for THEIA S2 data')
    
    parser.add_argument('--applocation', help='The path where the sen2agri is built', default="", required=False)
    parser.add_argument('--syntdate', help='L3A synthesis date', required=True)
    parser.add_argument('--synthalf', help='Half synthesis', required=True)
    parser.add_argument('--input', help='The list of products xml descriptors', required=True, nargs='+')
    parser.add_argument('--res', help='The requested resolution in meters', required=True)
    parser.add_argument('--outdir', help="Output directory", required=True)
    parser.add_argument('--wdir', help="Working directory", required=False)
    parser.add_argument('--bandsmap', help="Bands mapping file location", required=True)
       
    args = parser.parse_args()
    
    syntDate = args.syntdate
    syntHalf = args.synthalf
    
    inputList = args.input
    
    resolution = args.res
    bandsMap = args.bandsmap
    appLocation = args.applocation
    outDir = args.outdir
    if args.wdir:
        wDir = args.wdir
    else:
        wDir=outDir
    
    #defintion for filenames
    outMasks = os.path.join(wDir, 'masksCLD_WAT_SNW.tif')
    outImgBands =os.path.join(wDir, 'res' + resolution + '.tif')
    outCld = os.path.join(wDir, 'cld' + resolution + '.tif')
    outWat = os.path.join(wDir, 'wat' + resolution + '.tif')
    outSnow = os.path.join(wDir, 'snow' + resolution + '.tif')
    outAot = os.path.join(wDir, 'aot' + resolution + '.tif')
    outWeightAotFile = os.path.join(wDir, 'WeightAot#.tif')
    outWeightCloudFile = os.path.join(wDir, 'WeightCloud#.tif')
    outTotalWeightFile = os.path.join(wDir, 'WeightTotal#.tif')
    outL3AFile = os.path.join(outDir, 'L3AResult#_' + resolution + 'M.tif')
    
    WEIGHT_AOT_MIN="0.33"
    WEIGHT_AOT_MAX="1"
    AOT_MAX="0.8"
    
    COARSE_RES="240"
    SIGMA_SMALL_CLD="2"
    SIGMA_LARGE_CLD="10"
    
    WEIGHT_DATE_MIN="0.5"
    
    outWeights = os.path.join(outDir, 'L3AResult#_weights.tif')
    outRefls = os.path.join(outDir, 'L3AResult#_refls.tif')
    outFlags = os.path.join(outDir, 'L3AResult#_flags.tif')
    
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
        
        edgMask = os.path.join(os.path.dirname(xml),
                               'MASKS',
                               string.replace(
                                   os.path.basename(xml),
                                   'MTD_ALL.xml', 
                                   'EDG_R1.tif')
                               )
        freVRT =  os.path.join(os.path.dirname(xml),
                               string.replace(
                                   os.path.basename(xml),
                                   'MTD_ALL.xml', 
                                   'FRE.vrt')
                               )
    
        runCmd(["otbcli", "MaskHandler",
                appLocation,
                "-xml", xml,
                "-out", outMasks,
                "-sentinelres", resolution])
    
        counterString = str(i)
        cur_L3A_w=outWeights.replace("#", counterString)
        cur_L3A_refl=outRefls.replace("#", counterString)
        cur_L3A_flag=outFlags.replace("#", counterString)
        
        out_w_Aot=outWeightAotFile.replace("#", counterString)
        out_w_Cloud=outWeightCloudFile.replace("#", counterString)
        out_w_Total=outTotalWeightFile.replace("#", counterString)
       
        runCmd(["otbcli", "CompositePreprocessing2",
                appLocation,
                "-xml", xml,
                "-bmap", bandsMap,
                "-res", resolution,
                "-msk", outMasks,
                "-outcmres", outCld,
                "-outwmres", outWat,
                "-outsmres", outSnow,
                "-outaotres", outAot])
    
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
                    "-cl2arefl", freVRT,
                    "-cl2acsm", outCld,
                    "-cl2awm", outWat,
                    "-cl2asm", outSnow,
                    "-cl2aw", out_w_Total,
                    "-cl2aedg", edgMask,
                    "-bluebandidx", "0",
                    "-cl3arefl", cur_L3A_refl,
                    "-cl3aflag", cur_L3A_flag,
                    "-cl3aw", cur_L3A_w])
        else:
            runCmd(["otbcli", "UpdateSynthesisS2",
                    appLocation,
                    "-cl2arefl", freVRT,
                    "-cl2acsm", outCld,
                    "-cl2awm", outWat,
                    "-cl2asm", outSnow,
                    "-cl2aw", out_w_Total,
                    "-cl2aedg", edgMask,
                    "-bluebandidx", "0",
                    "-cl3arefl", cur_L3A_refl,
                    "-cl3aflag", cur_L3A_flag,
                    "-cl3aw", cur_L3A_w,
                    "-pl3aw", prev_L3A_w,
                    "-pl3arefl", prev_L3A_refl,
                    "-pl3aflag", prev_L3A_flag])
        
        prev_L3A_w = cur_L3A_w
        prev_L3A_refl = cur_L3A_refl
        prev_L3A_flag = cur_L3A_flag
        
        # Clean
        os.remove(out_w_Aot)
        os.remove(out_w_Cloud)
        os.remove(out_w_Total)
        
        i += 1
    
    if i == 0:
        print("No L2A products found !")
        exit(1)
    
    print("Processing finished: " + str(datetime.datetime.now()))
    print("Total execution time: {}".format(datetime.timedelta(seconds=(time.time() - start))))
    