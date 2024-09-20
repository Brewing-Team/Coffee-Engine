//
// Created by mdoradom on 20/09/24.
//

#pragma once
#include "Panel.h"
#include "TeaEngine/Scene/Scene.h"

namespace Tea {

  class OutputPanel : public Panel {
  public:
    OutputPanel() = default;

    void OnImGuiRender() override;

  private:

  };

} // Tea