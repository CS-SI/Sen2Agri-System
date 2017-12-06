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
 : m_OutputSize(0),
   m_NbBandsRefl(0),
   m_Duration(-1000),
   m_BlueBandIdx(-1)
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
  for (unsigned int i = 0; i < out.GetSize(); ++i )
    {
    out[i] = 0;
    }
  return out;
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
