/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <filamat/MaterialEditor.h>

#include "eiff/ChunkContainer.h"
#include "eiff/DictionaryTextChunk.h"
#include "eiff/LineDictionary.h"
#include "eiff/MaterialTextChunk.h"

#include <vector>

namespace filamat {

MaterialEditor::MaterialEditor(const Package& original) : mOriginal(original) {

}

bool MaterialEditor::applyShaderEdit(filament::backend::ShaderModel shaderModel, uint8_t variant,
        filament::backend::ShaderType stage, const char* source) {
#ifndef FILAMAT_LITE

    std::vector<TextEntry> glslEntries;
    LineDictionary glslDictionary;

    ChunkContainer container;

    const auto& dictionaryChunk = container.addChild<filamat::DictionaryTextChunk>(
            std::move(glslDictionary), ChunkType::DictionaryGlsl);

    container.addChild<MaterialTextChunk>(std::move(glslEntries),
            dictionaryChunk.getDictionary(), ChunkType::MaterialGlsl);

#endif
    return true;
}

const Package& MaterialEditor::getEditedPackage() const {
    return mEdited;
}

} // namespace filamat
