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

  void SetNbBandsRefl(unsigned int nbBandsRefl)
  {
	  m_NbBandsRefl = nbBandsRefl;
	  m_OutputSize = 2 * m_NbBandsRefl + 2;
  }

  TOutput operator ()(const TInput& in) const;

private:

  int m_Duration;
  int m_BlueBandIdx;
  unsigned int m_NbBandsRefl;
  unsigned int m_OutputSize;
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
