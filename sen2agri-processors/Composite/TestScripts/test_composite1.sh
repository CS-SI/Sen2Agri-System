#! /bin/sh

if [ $# -lt 2 ]
then
  echo "Usage: $0 <in folder name> <out folder name>"
  exit
fi

#IN_FOLDER="/media/sf_2_Sen2Agri/8_ProductSamples/2_FromMACCS"
#OUT_FOLDER="/media/sf_2_Sen2Agri/8_ProductSamples/4_TestOutputs"
IN_FOLDER=$1
OUT_FOLDER=$2

COMPOSITE_OTB_LIBS_ROOT="../../../sen2agri-processors-build/Composite"
WEIGHT_OTB_LIBS_ROOT="$COMPOSITE_OTB_LIBS_ROOT/WeightCalculation"

INPUT_S2_XML_FILE="$IN_FOLDER/S2A_OPER_SSC_L2VALD_15SVD____20091211.HDR"
INPUT_S2_IMG_FILE="$IN_FOLDER/S2A_OPER_SSC_L2VALD_15SVD____20091211.DBL.DIR/S2A_OPER_SSC_PDTIMG_L2VALD_15SVD____20091211_FRE_R1.DBL.TIF"
INPUT_S2_CLD_FILE="$IN_FOLDER/S2A_OPER_SSC_L2VALD_15SVD____20091211.DBL.DIR/S2A_OPER_SSC_PDTANX_L2VALD_15SVD____20091211_CLD_R1.DBL.TIF"
INPUT_S2_AOT_FILE="$IN_FOLDER/S2A_OPER_SSC_L2VALD_15SVD____20091211.DBL.DIR/S2A_OPER_SSC_PDTANX_L2VALD_15SVD____20091211_ATB_R1.DBL.TIF"

OUT_WEIGHT_AOT_FILE="$OUT_FOLDER/WeightAot.tiff"
OUT_WEIGHT_CLOUD_FILE="$OUT_FOLDER/WeightCloud.tiff"
OUT_TOTAL_WEIGHT_FILE="$OUT_FOLDER/WeightTotal.tiff"
OUT_L3A_FILE="$OUT_FOLDER/L3AResult.tiff"

WEIGHT_AOT_MIN="0.33"
WEIGHT_AOT_MAX="1"
AOT_MAX="50"

COARSE_RES="240"
SIGMA_SMALL_CLD="10"
SIGMA_LARGE_CLD="50"

WEIGHT_SENSOR="0.33"
L3A_DATE="20140502"
HALF_SYNTHESIS="50"
WEIGHT_DATE_MIN="0.10"

MY_PWD=`pwd`

echo "Executing from $MY_PWD"

otbcli WeightAOT $WEIGHT_OTB_LIBS_ROOT/WeightAOT/ -in $INPUT_S2_AOT_FILE -xml $INPUT_S2_XML_FILE -waotmin $WEIGHT_AOT_MIN -waotmax $WEIGHT_AOT_MAX -aotmax $AOT_MAX -out $OUT_WEIGHT_AOT_FILE

#otbcli WeightOnClouds $WEIGHT_OTB_LIBS_ROOT/WeightOnClouds/ -incldmsk $INPUT_S2_CLD_FILE -coarseres $COARSE_RES -sigmasmallcld $SIGMA_SMALL_CLD -sigmalargecld $SIGMA_LARGE_CLD -out $OUT_WEIGHT_CLOUD_FILE

#otbcli TotalWeight $WEIGHT_OTB_LIBS_ROOT/TotalWeight/ -xml $INPUT_S2_XML_FILE -waotfile $OUT_WEIGHT_AOT_FILE -wcldfile $OUT_WEIGHT_CLOUD_FILE -wsensor $WEIGHT_SENSOR -l3adate $L3A_DATE -halfsynthesis $HALF_SYNTHESIS -wdatemin $WEIGHT_DATE_MIN -out $OUT_TOTAL_WEIGHT_FILE

#otbcli UpdateSynthesis $COMPOSITE_OTB_LIBS_ROOT/UpdateSynthesis/ -in $INPUT_S2_IMG_FILE -res 10 -xml $INPUT_S2_XML_FILE -csm $INPUT_S2_CLD_FILE -wl2a $OUT_TOTAL_WEIGHT_FILE -out $OUT_L3A_FILE