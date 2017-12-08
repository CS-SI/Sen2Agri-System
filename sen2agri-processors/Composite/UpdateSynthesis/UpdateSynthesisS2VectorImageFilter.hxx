/*=========================================================================
  *
  * Program:      Sen2agri-Processors
  * Language:     C++
  * Copyright:    2017, CS SI, mickael.savinaud@c-s.fr
  * See COPYRIGHT file for details.
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.

 =========================================================================*/


#ifndef __UpdateSynthesisS2VectorImageFilter_txx
#define __UpdateSynthesisS2VectorImageFilter_txx

#include "UpdateSynthesisS2VectorImageFilter.h"

namespace otb
{

namespace Functor
{

template <class TInput, class TOutput>
UpdateSynthesisS2Functor<TInput, TOutput>
::UpdateSynthesisS2Functor()
 : m_Duration(-1000),
   m_BlueBandIdx(-1), m_NbBandsRefl(0),
   m_OutputSize(0),
   m_SnowMaskBandIndex(0),
   m_WaterMaskBandIndex(0),
   m_CloudMaskBandIndex(0),
   m_WeightBandIndex(0),
   m_EdgeMaskBandIndex(0),
   m_FirstCurrentL2AReflIdx(0),
   m_FirstPreviousL3AReflIdx(0),
   m_FirstPreviousL3AWeightIdx(0),
   m_FirstPreviousL3ADatesIdx(0),
   m_FirstPreviousL3AFlagIdx(0),
   m_FirstNewL3AReflIdx(0),
   m_FirstNewL3AWeightIdx(0),
   m_FirstNewL3ADatesIdx(0),
   m_FirstNewL3AFlagIdx(0),
   m_InitMode(false)

{
}

template <class TInput, class TOutput>
UpdateSynthesisS2Functor<TInput, TOutput>
::~UpdateSynthesisS2Functor()
{
}

template <class TInput, class TOutput>
unsigned int
UpdateSynthesisS2Functor<TInput, TOutput>
::GetOutputSize() const
{
  return m_OutputSize;
}

template <class TInput, class TOutput>
bool
UpdateSynthesisS2Functor<TInput, TOutput>
::operator != (const Self& other) const
{
  return true;
}

template <class TInput, class TOutput>
bool
UpdateSynthesisS2Functor<TInput, TOutput>
::operator == (const Self& other) const
{
  return false;
}

template <class TInput, class TOutput>
typename UpdateSynthesisS2Functor<TInput, TOutput>::OutputType
UpdateSynthesisS2Functor<TInput, TOutput>
::operator ()(const InputType& in) const
{
  OutputType out(m_OutputSize);
  out.Fill(0.0);

  if(this->IsLandPixel(in)) {

    out = HandleLandPixel(in);
  }
  else {
    if(this->IsCloudPixel(in)) {
      // if pixel is cloud or shadow *pixel never observed cloud snow or water free

      out = HandleCloudOrShadowPixel(in);
    }
    else if(this->IsSnowPixel(in) || this->IsWaterPixel(in)) {

      // if pixel is snow or water *replace the reflectance value
      out = HandleSnowOrWaterPixel(in);
    }

  }

  return out;
}

template< class TInput, class TOutput>
bool UpdateSynthesisS2Functor<TInput,TOutput>::IsLandPixel(const TInput & in) const
{
    return (!IsSnowPixel(in) && !IsWaterPixel(in) && !IsCloudPixel(in));
}

template< class TInput, class TOutput>
bool UpdateSynthesisS2Functor<TInput,TOutput>::IsSnowPixel(const TInput & in) const
{
  // No data should not be considered as snow
  if(IsNoDataValue(in)) {
      return false;
  }
  return (in[m_SnowMaskBandIndex] != 0.0); // TODO use itkAlmostEqualsFunction(a,b)
}

template< class TInput, class TOutput>
bool UpdateSynthesisS2Functor<TInput,TOutput>::IsWaterPixel(const TInput & in) const
{
  // No data should not be considered as water
  if(IsNoDataValue(in)) {
      return false;
  }
  return (in.GetElement(m_WaterMaskBandIndex) != 0.0); // TODO use itkAlmostEqualsFunction(a,b)
}

template< class TInput, class TOutput>
bool UpdateSynthesisS2Functor<TInput,TOutput>::IsCloudPixel(const TInput & in) const
{
  // No data should not be considered as cloud
  if(IsNoDataValue(in)) {
      return false;
  }
  return (in[m_CloudMaskBandIndex] != 0.0); // TODO use itkAlmostEqualsFunction(a,b)
}

template< class TInput, class TOutput>
bool UpdateSynthesisS2Functor<TInput,TOutput>::IsNoDataValue(const TInput & in) const
{
    return (in[m_EdgeMaskBandIndex] != 0.0); // TODO use itkAlmostEqualsFunction(a,b)
}

template< class TInput, class TOutput>
TOutput UpdateSynthesisS2Functor<TInput,TOutput>::HandleLandPixel(const TInput & in) const
{
  OutputType out(m_OutputSize);
  out.Fill(0.0);

  if (IsNoDataValue(in)){  // si le pixel est flagué dans EDG (NO_DATA)
    // on garde les valeurs précédentes
    for (unsigned int i = 0; i < m_NbBandsRefl; ++i ) {
      out[m_FirstNewL3AReflIdx + i] = GetPrevL3AReflValue(in, i);
      out[m_FirstNewL3AWeightIdx + i] = GetPrevL3AWeightValue(in, i);
      //out[m_FirstNewL3ADatesIdx + i] = 0.0 ; //in[m_FirstPreviousL3ADatesIdx + i];
    }
    out[m_FirstNewL3AFlagIdx] = GetPrevL3AFlagValue(in);
  }
  else{ // si le pixel est flagué comme LAND
    // on calcule les nouvelles valeurs
    typename OutputType::ComponentType curWeight = in[m_WeightBandIndex];
    for (unsigned int i = 0; i < m_NbBandsRefl; ++i ) {
      typename OutputType::ComponentType prevWeight = GetPrevL3AWeightValue(in, i);
      typename OutputType::ComponentType newWeight = prevWeight + curWeight ;
      out[m_FirstNewL3AReflIdx + i] = 10000.0 * ( (prevWeight * GetPrevL3AReflValue(in, i) + curWeight * (in[m_FirstCurrentL2AReflIdx+ i] / 10000.0)) / newWeight);
      out[m_FirstNewL3AWeightIdx + i] = 0.0; //(prevWeight * in[m_FirstPreviousL3ADatesIdx + i] + curWeight * m_Duration)/ newWeight;
      //out[m_FirstNewL3ADatesIdx + i] = newWeight;
    }
    out[m_FirstNewL3AFlagIdx] = 4 ; // LAND
  }
  return out;
}

template< class TInput, class TOutput>
TOutput UpdateSynthesisS2Functor<TInput,TOutput>::HandleCloudOrShadowPixel(const TInput & in) const
{
  OutputType out(m_OutputSize);
  out.Fill(0.0);

  if (in[m_FirstPreviousL3AFlagIdx] == 0) { // composite pixel is no-data
    for (unsigned int i = 0; i < m_NbBandsRefl; ++i ) {
      out[m_FirstNewL3AReflIdx + i] = in[m_FirstCurrentL2AReflIdx + i];
      out[m_FirstNewL3AWeightIdx + i] = 0.0 ;
      //out[m_FirstNewL3ADatesIdx + i] = 0.0 ;
    }
    out[m_FirstNewL3AFlagIdx] = 1 ; // CLOUD
  }

  if (in[m_FirstPreviousL3AFlagIdx] == 1) { // composite pixel is cloud or shadow
    if ( in[m_BlueBandIdx] < GetPrevL3AReflValue(in, m_BlueBandIdx) ){
      for (unsigned int i = 0; i < m_NbBandsRefl; ++i ) {
        out[m_FirstNewL3AReflIdx + i] = in[m_FirstCurrentL2AReflIdx + i];
        out[m_FirstNewL3AWeightIdx + i] = 0.0 ;
        //out[m_FirstNewL3ADatesIdx + i] = 0.0 ;
      }
      out[m_FirstNewL3AFlagIdx] = 1 ; // CLOUD
    }
    else {
      for (unsigned int i = 0; i < m_NbBandsRefl; ++i ) {
        out[m_FirstNewL3AReflIdx + i] = GetPrevL3AReflValue(in, i);
        out[m_FirstNewL3AWeightIdx + i] = 0.0 ;
        //out[m_FirstNewL3ADatesIdx + i] = 0.0 ;
      }
      out[m_FirstNewL3AFlagIdx] = GetPrevL3AFlagValue(in);
    }
  }

  return out;
}

template< class TInput, class TOutput>
TOutput UpdateSynthesisS2Functor<TInput,TOutput>::HandleSnowOrWaterPixel(const TInput & in) const
{
  OutputType out(m_OutputSize);
  out.Fill(0.0);

  if (IsWaterPixel(in) )
    out[m_FirstNewL3AFlagIdx] = 3 ; // WATER
  else
    out[m_FirstNewL3AFlagIdx] = 2 ; // SNOW

  for (unsigned int i = 0; i < m_NbBandsRefl; ++i ) {
    // Si le pixel n'a jamais été vu sans cloud, water or snow
    if (in[m_FirstPreviousL3AWeightIdx + i] == 0.0){ // TODO use itkAlmostEqual
      out[m_FirstNewL3AReflIdx + i] = in[m_FirstCurrentL2AReflIdx + i];
    }
    else {
      out[m_FirstNewL3AReflIdx + i] = in[m_FirstPreviousL3AReflIdx + i];
    }
    out[m_FirstNewL3AWeightIdx + i] = in[m_FirstPreviousL3AWeightIdx + i]; // 0 or the previous value
    //out[m_FirstNewL3ADatesIdx + i] = 0.0 ;
  }

  return out;
}

template< class TInput, class TOutput>
float UpdateSynthesisS2Functor<TInput,TOutput>::GetPrevL3AWeightValue(const TInput & in, int offset) const
{
    if(m_InitMode)
      return 0.0;
    else
      return in[m_FirstPreviousL3AWeightIdx + offset];
}

template< class TInput, class TOutput>
float UpdateSynthesisS2Functor<TInput,TOutput>::GetPrevL3ADatesValue(const TInput & in, int offset) const
{
  if(m_InitMode)
    return 0.0; // TBD
  else
    return in[m_FirstPreviousL3ADatesIdx + offset];
}

template< class TInput, class TOutput>
float UpdateSynthesisS2Functor<TInput,TOutput>::GetPrevL3AReflValue(const TInput & in, int offset) const
{
  if(m_InitMode)
    return -1.0; // value already divided by QUANTIFICATION VALUE
  else
    return in[m_FirstPreviousL3AReflIdx + offset] / 10000.0;
}

template< class TInput, class TOutput>
float UpdateSynthesisS2Functor<TInput,TOutput>::GetPrevL3AFlagValue(const TInput & in) const
{
  if(m_InitMode)
    return 0.0; // NODATA
  else
    return in[m_FirstPreviousL3AFlagIdx];
}


}

template <class TInputImage, class TOutputImage>
UpdateSynthesisS2VectorImageFilter<TInputImage, TOutputImage>
::UpdateSynthesisS2VectorImageFilter()
{
}

template <class TInputImage, class TOutputImage>
UpdateSynthesisS2VectorImageFilter<TInputImage, TOutputImage>
::~UpdateSynthesisS2VectorImageFilter()
{
}

template <class TInputImage, class TOutputImage>
void
UpdateSynthesisS2VectorImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace

#endif
