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
#include <map>

struct THEIADatasetIdentification
{
    std::string Authority;
    std::string Project;
    std::string Producer;
};

typedef std::map<std::string, std::string> THEIAMask;

struct THEIAProductOrganisation
{
    std::string QLK;
    std::vector<std::string> SRE;
    std::vector<std::string> FRE;
    std::map<std::string, THEIAMask> MASKS;

};

struct THEIAMetadata
{
    THEIADatasetIdentification datasetIdentification;
    THEIAProductOrganisation productOrganisation;
};


