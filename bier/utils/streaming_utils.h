/*
   Copyright 2019 Igor Kholopov

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once
#include <ostream>

namespace bier {

template <typename TIterable, typename F>
static std::ostream& JoinWithSeparator(const std::string& separator, std::ostream& stream,
                                       const TIterable& iterable, F op) {
    for (auto it = iterable.begin(); it != iterable.end(); ++it) {
        op(*it);
        auto copy = it;
        if (++copy != iterable.end()) {
            stream << separator;
        }
    }

    return stream;
}


}   // bier
