/*=========================================================================
  *
  * Program:      Sen2agri-Processors
  * Language:     C++
  * Copyright:    2015-2016, CS Romania, office@c-s.ro
  * See COPYRIGHT file for details.
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.

 =========================================================================*/
 
#ifndef MASKEXTRACTORFILTER_H
#define MASKEXTRACTORFILTER_H

#include "itkBinaryFunctorImageFilter.h"
#include "itkThresholdImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbWrapperTypes.h"
#include "MaskHandlerFunctor.h"
#include "MACCSMetadataReader.hpp"
#include "SPOT4MetadataReader.hpp"

#include "itkNumericTraits.h"
#include "otbImage.h"

namespace otb
{

template <class TImageType, class TOutputImageType>
class ITK_EXPORT MaskExtractorFilter :
  public itk::ImageToImageFilter<TImageType, TOutputImageType>
{
public:
  typedef MaskExtractorFilter                     Self;
  typedef itk::ImageToImageFilter<TImageType, TOutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through object factory */
  itkNewMacro(Self);

  /** Run-time type information */
  itkTypeMacro(MaskExtractorFilter, itk::ImageToImageFilter);

  /* set input xml to read the cloud and water/snow masks */
  void SetInputXml(std::string xml);

  /** Display */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  //virtual void UpdateOutputInformation() ITK_OVERRIDE;

  typedef typename TImageType::PixelType PixelType;
  typedef typename TOutputImageType::PixelType OutputPixelType;

  void SetBitsMask(int cloudMask, int waterMask, int snowMask);

protected:

  MaskExtractorFilter();

protected:
  typedef MaskHandlerFunctor <PixelType, PixelType, OutputPixelType> MaskHandlerFunctorType;
  typedef itk::BinaryFunctorImageFilter< TImageType, TImageType, TOutputImageType,
                            MaskHandlerFunctorType > FunctorFilterType;

  typedef otb::ImageList<TImageType>  ImageListType;
  typedef ImageListToVectorImageFilter<ImageListType,
                                       PixelType >                   ListConcatenerFilterType;

  virtual void GenerateData() ITK_OVERRIDE;
  virtual void UpdateOutputInformation() ITK_OVERRIDE;


private:

  MaskExtractorFilter(Self &);   // intentionally not implemented
  void operator =(const Self&);          // intentionally not implemented

  typename FunctorFilterType::Pointer          m_MaskHandlerFunctor;
  MaskHandlerFunctorType          m_Functor;


  PixelType m_Threshold;
};

} /* namespace otb */

#endif

