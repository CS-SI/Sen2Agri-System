/*=========================================================================
  *
  * Program:      Sen2agri-Processors
  * Language:     C++
  * Copyright:    2017, CS SI
  * See COPYRIGHT file for details.
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.

 =========================================================================*/

#pragma once

#include <string>
#include <vector>

struct THEIAFiles
{
    std::string MTD_ALL;
    std::string QLK_ALL;
    std::vector<std::string> SRE;
    std::vector<std::string> FRE;
    std::string ATB_R1;
    std::string ATB_R2;
    std::string MASK_EDG_R1;
    std::string MASK_EDG_R2;
    std::string MASK_SAT_R1;
    std::string MASK_SAT_R2;
    std::string MASK_CLM_R1;
    std::string MASK_CLM_R2;
    std::string MASK_MG2_R1;
    std::string MASK_MG2_R2;
    std::string MASK_IAO_R1;
    std::string MASK_IAO_R2;
};

struct THEIAMetadata
{
    std::string MissionName;
    THEIAFiles Files;
};


