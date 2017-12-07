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

#ifndef __UpdateSynthesisS2VectorImageFilter_h
#define __UpdateSynthesisS2VectorImageFilter_h

#include "itkMacro.h"
#include "otbUnaryFunctorImageFilter.h"


namespace otb
{

namespace Functor {

/** \class UpdateSynthesisS2Functor
 *
 * \brief TODO
 *
 */
template<class TInput, class TOutput>
class UpdateSynthesisS2Functor
{
public:
  typedef UpdateSynthesisS2Functor<TInput, TOutput> Self;

  typedef TInput     InputType;
  typedef TOutput    OutputType;

  UpdateSynthesisS2Functor();
  virtual ~UpdateSynthesisS2Functor();

  unsigned int GetOutputSize() const;

  bool operator !=(const UpdateSynthesisS2Functor& other) const;

  bool operator ==(const UpdateSynthesisS2Functor& other) const;

  void SetBlueBandIdx(unsigned int blueBandIdx)
  {
  	m_BlueBandIdx = static_cast<int>(blueBandIdx);
  }

  void SetDuration(int duration)
  {
  	m_Duration = duration;
  }

  void InitModeOn()
  {
    m_InitMode = true;
  }

  void SetNbBandsRefl(unsigned int nbBandsRefl)
  {
	  m_NbBandsRefl = nbBandsRefl;
	  m_OutputSize = 3 * m_NbBandsRefl + 1;
	  m_FirstCurrentL2AReflIdx = 0;
	  m_CloudMaskBandIndex = m_NbBandsRefl;
	  m_WaterMaskBandIndex = m_CloudMaskBandIndex + 1;
	  m_SnowMaskBandIndex = m_WaterMaskBandIndex + 1;
	  m_WeightBandIndex = m_SnowMaskBandIndex + 1;
	  m_EdgeMaskBandIndex = m_WeightBandIndex + 1;
	  m_FirstPreviousL3AReflIdx = m_EdgeMaskBandIndex + 1;
      m_FirstPreviousL3AWeightIdx = m_FirstPreviousL3AReflIdx + m_NbBandsRefl;
      m_FirstPreviousL3ADatesIdx = m_FirstPreviousL3AWeightIdx + m_NbBandsRefl;
      m_FirstPreviousL3AFlagIdx = m_FirstPreviousL3ADatesIdx + 1;

      m_FirstNewL3AReflIdx = 0;
      m_FirstNewL3AWeightIdx = m_NbBandsRefl;
      m_FirstNewL3ADatesIdx = m_FirstNewL3AWeightIdx + m_NbBandsRefl;
      m_FirstNewL3AFlagIdx = m_FirstNewL3ADatesIdx + m_NbBandsRefl;

      std::cout << "m_FirstCurrentL2AReflIdx: " << m_FirstCurrentL2AReflIdx << std::endl;
      std::cout << "m_CloudMaskBandIndex: " << m_CloudMaskBandIndex << std::endl;
      std::cout << "m_WaterMaskBandIndex: " << m_WaterMaskBandIndex << std::endl;
      std::cout << "m_SnowMaskBandIndex: " << m_SnowMaskBandIndex << std::endl;
      std::cout << "m_WeightBandIndex: " << m_WeightBandIndex << std::endl;
      std::cout << "m_EdgeMaskBandIndex: " << m_EdgeMaskBandIndex << std::endl;
      std::cout << "m_FirstPreviousL3AReflIdx: " << m_FirstPreviousL3AReflIdx << std::endl;
      std::cout << "m_FirstPreviousL3AWeightIdx: " << m_FirstPreviousL3AWeightIdx << std::endl;
      std::cout << "m_FirstPreviousL3ADatesIdx: " << m_FirstPreviousL3ADatesIdx  << std::endl;
      std::cout << "m_FirstPreviousL3AFlagIdx: " <<  m_FirstPreviousL3AFlagIdx << std::endl;
      std::cout << "m_FirstNewL3AReflIdx: " << m_FirstNewL3AReflIdx << std::endl;
      std::cout << "m_FirstNewL3AWeightIdx: " << m_FirstNewL3AWeightIdx << std::endl;
      std::cout << "m_FirstNewL3ADatesIdx: " <<m_FirstNewL3ADatesIdx << std::endl;
      std::cout << "m_FirstNewL3AFlagIdx: " << m_FirstNewL3AFlagIdx << std::endl;
//      std::cout << "" << << std::endl;
//      std::cout << "" << << std::endl;
//      std::cout << "" << << std::endl;
//      std::cout << "" << << std::endl;
//      std::cout << "" << << std::endl;
//      std::cout << "" << << std::endl;
//      std::cout << "" << << std::endl;
//      std::cout << "" << << std::endl;
//      std::cout << "" << << std::endl;
//      std::cout << "" << << std::endl;
//      std::cout << "" << << std::endl;

  }

  TOutput operator ()(const TInput& in) const;

private:

  int m_Duration;
  int m_BlueBandIdx;
  unsigned int m_NbBandsRefl;
  unsigned int m_OutputSize;
  unsigned int m_SnowMaskBandIndex;
  unsigned int m_WaterMaskBandIndex;
  unsigned int m_CloudMaskBandIndex;
  unsigned int m_WeightBandIndex;
  unsigned int m_EdgeMaskBandIndex;
  unsigned int m_FirstCurrentL2AReflIdx;
  unsigned int m_FirstPreviousL3AReflIdx;
  unsigned int m_FirstPreviousL3AWeightIdx;
  unsigned int m_FirstPreviousL3ADatesIdx;
  unsigned int m_FirstPreviousL3AFlagIdx;
  unsigned int m_FirstNewL3AReflIdx;
  unsigned int m_FirstNewL3AWeightIdx;
  unsigned int m_FirstNewL3ADatesIdx;
  unsigned int m_FirstNewL3AFlagIdx;
  bool m_InitMode;

  bool IsLandPixel(const TInput & in) const;
  bool IsSnowPixel(const TInput & in) const;
  bool IsWaterPixel(const TInput & in) const;
  bool IsCloudPixel(const TInput & in) const;
  bool IsNoDataValue(const TInput & in) const;

  OutputType HandleLandPixel(const TInput & in) const;
  OutputType HandleSnowOrWaterPixel(const TInput & in) const;
  OutputType HandleCloudOrShadowPixel(const TInput & in) const;

  float GetPrevL3AWeightValue(const TInput & in, int offset) const;
  float GetPrevL3ADatesValue(const TInput & in, int offset) const;
  float GetPrevL3AReflValue(const TInput & in, int offset) const;
  float GetPrevL3AFlagValue(const TInput & in) const;

};

}

/** \class NCLSUnmixingImageFilter
 *
 * \brief TODO
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT UpdateSynthesisS2VectorImageFilter :
  public otb::UnaryFunctorImageFilter<TInputImage, TOutputImage,
      Functor::UpdateSynthesisS2Functor<  typename TInputImage::PixelType,
	  	  	  	  	  	  	  	  	  	  typename TOutputImage::PixelType> >
{
public:
  /** Standard class typedefs. */
  typedef UpdateSynthesisS2VectorImageFilter Self;
  typedef otb::UnaryFunctorImageFilter
     <TInputImage,
      TOutputImage,
      Functor::UpdateSynthesisS2Functor<
          typename TInputImage::PixelType,
          typename TOutputImage::PixelType> >	Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef Functor::UpdateSynthesisS2Functor<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType> FunctorType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UpdateSynthesisS2VectorImageFilter, otb::UnaryFunctorImageFilter);

  /** Pixel types. */
  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;

  void SetDuration( int val )
  {
    this->GetFunctor().SetDuration(val);
    this->Modified();
  }

  void SetBlueBandIdx( unsigned int val )
  {
      this->GetFunctor().SetBlueBandIdx(val);
      this->Modified();
  }

  void SetNbBandsRefl( unsigned int val )
  {
      this->GetFunctor().SetNbBandsRefl(val);
      this->Modified();
  }

  void InitModeOn()
  {
    this->GetFunctor().InitModeOn();
    this->Modified();
  }

protected:
  UpdateSynthesisS2VectorImageFilter();

  virtual ~UpdateSynthesisS2VectorImageFilter();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  UpdateSynthesisS2VectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "UpdateSynthesisS2VectorImageFilter.hxx"
#endif



#endif
