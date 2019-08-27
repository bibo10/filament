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

#ifndef TNT_FILAMAT_MATERIAL_EDITOR_H
#define TNT_FILAMAT_MATERIAL_EDITOR_H

#include <filamat/Package.h>

#include <backend/DriverEnums.h>

namespace filamat {

/**
 * Allows injection of shader code into existing material packages. Designed for internal use.
 * This class is not implemented when FILAMAT_LITE is defined.
 */
class MaterialEditor {
public:
    MaterialEditor(const Package& original);

    bool applyShaderEdit(filament::backend::ShaderModel shaderModel, uint8_t variant,
            filament::backend::ShaderType stage, const char* source);

    const Package& getEditedPackage() const;

private:
    Package mEdited;
    const Package& mOriginal;
};

} // namespace filamat

#endif // TNT_FILAMAT_MATERIAL_EDITOR_H
