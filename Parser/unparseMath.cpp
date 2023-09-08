#include "unparseMath.h"
#include "equationException.h"
#include <cmath>

double xValue;

double resultExpression(AST *expression, double xVal)
{
	xValue = xVal;

	return resultExpr(expression);
}

double resultExpr(AST *expr)
{
	if(expr == NULL){
		return 0;
	}
	double res = 0;
	switch (expr->type_tag)
	{
	case bin_expr_ast:
		res = resultBinExpr(expr);
		break;
	case func_call_ast:
		res = resultFuncCall(expr);
		break;
	case ident_ast:
		res = resultIdent(expr);
		break;
	case number_ast:
		res = resultNum(expr);
		break;
	default:
		res = 0;
		break;
	}

	return res;
}

double resultBinExpr(AST *binExpr)
{
	double left = resultExpr(binExpr->data.bin_expr.leftexp);

	double right = resultExpr(binExpr->data.bin_expr.rightexp);

	double res = 0;
	switch (binExpr->data.bin_expr.arith_op)
	{
	case addop:
		res = left + right;
		break;
	case subop:
		res = left - right;
		break;
	case multop:
		res = left * right;
		break;
	case divop:
		res = left / right;
		break;
	}

	return res;
}

double resultFuncCall(AST *funcCall)
{

	switch (funcCall->data.func_call.func)
	{
	case sin_f:
		return resultSin(funcCall);
		break;
	default:
		throw EquationException("Unrecognized function", funcCall->index);
	}
	return 0;
}

double resultSin(AST *sinFunc){
	AST *param = ast_list_first(sinFunc->data.func_call.parameters);
	double exprResult = resultExpr(sinFunc->data.func_call.parameters);

	return sin(exprResult);

}

double resultIdent(AST *ident)
{
	string str(ident->data.ident.name);
	if (str != "x")
	{
		throw EquationException("Unknown variable: " + str, ident->index);
	}

	return xValue;
}

double resultNum(AST *num)
{
	return num->data.number.value;
}