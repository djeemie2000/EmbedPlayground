#pragma once

#include <vector>
#include <string>
#include "SelectableOperator.h"

#include "RampUp.h"
#include "RampDown.h"
#include "Triangle.h"
#include "FullPseudoSin.h"
#include "Square.h"
#include "InvSquare.h"
#include "SquareWave.h"
#include "RampUpDown.h"
#include "SquareQuadratic.h"
#include "Pulse.h"
#include "NoOp.h"

class CSelectableOperatorFactory
{
public:
    CSelectableOperatorFactory(){}

    static std::vector<std::string> SelectionList()
    {
        return {"RampUp", "RampDown", "Triangle", "FullPseudoSin", "PseudoSin", "Quadratic", "InvQuadratic", "Square", "SquareQuadratic", "RampUpDown", "Pulse+", "Pulse-", "NoOp" };
    }

    static CSelectableOperator<float> Create()
    {
        CSelectableOperator<float> Op;

        Op.Add(CRampUp<float>());
        Op.Add(CRampDown<float>());
        Op.Add(CTriangle<float>());
        Op.Add(CFullPseudoSin<float>());
        Op.Add(CPseudoSin<float>());
        Op.Add(CQuadratic<float>());
        Op.Add(CInvSquare<float>());
        Op.Add(CSquareWave<float>());
        Op.Add(CSquareQuadratic<float>());
        Op.Add(CRampUpDown<float>());
        Op.Add(CPulsePos<float>());
        Op.Add(CPulseNeg<float>());
        Op.Add(CNoOp<float>());

        Op.Select(0);

        return Op;
    }

    CSelectableOperator<float> CreateOperator() const
    {
        CSelectableOperator<float> Op;

        Op.Add(CRampUp<float>());
        Op.Add(CRampDown<float>());
        Op.Add(CTriangle<float>());
        Op.Add(CFullPseudoSin<float>());
        Op.Add(CPseudoSin<float>());
        Op.Add(CQuadratic<float>());
        Op.Add(CInvSquare<float>());
        Op.Add(CSquareWave<float>());
        Op.Add(CSquareQuadratic<float>());
        Op.Add(CRampUpDown<float>());
        Op.Add(CNoOp<float>());

        return Op;
    }
};


//CSelectableOperator<float> CSelectableOperatorFactory::CreateOperator() const
//{
//}
