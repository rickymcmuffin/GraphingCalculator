#include "unparseMath.h"
#include "equationException.h"
#include <cmath>

using namespace std;

double xValue;

double fValue;

double resultExpression(AST *expression, double xVal)
{
	return resultExpression(expression, xVal, 1);
}

double resultExpression(AST *expression)
{
	return resultExpression(expression, 1, 1);
}
double resultExpression(AST *expression, double xVal, double fVal)
{

	xValue = xVal;
	fValue = fVal;

	return resultExpr(expression);
}

double resultExpr(AST *expr)
{
	if (expr == NULL)
	{
		return 0;
	}
	double res = 0;
	switch (expr->type_tag)
	{
	case peacewise_ast:
		res = resultPeacewise(expr);
		break;
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

double resultPeacewise(AST *expr)
{
	cout << "resultPeacewsie"<<endl;
	AST_list condExprs = expr->data.peacewise.cond_exprs;

	while(!ast_list_is_empty(condExprs)){
		AST *curCond = ast_list_first(condExprs)->data.cond_expr.cond;
		if(resultCond(curCond)){
			return resultExpr(ast_list_first(condExprs)->data.cond_expr.expr);
		}
		condExprs = ast_list_rest(condExprs);
	}
	return 0;
}

bool resultCond(AST *cond)
{
	double leftResult = resultExpr(cond->data.bin_cond.leftexp);
	double rightResult = resultExpr(cond->data.bin_cond.rightexp);

	switch (cond->data.bin_cond.relop)
	{
	case eqop:
		return leftResult == rightResult;
		break;
	case neqop:
		return leftResult != rightResult;
		break;
	case ltop:
		return leftResult < rightResult;
		break;
	case leqop:
		return leftResult <= rightResult;
		break;
	case gtop:
		return leftResult > rightResult;
		break;
	case geqop:
		return leftResult >= rightResult;
		break;
	}
	return NULL;
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
	case pow_f:
		return resultPow(funcCall);
		break;
	case mod_f:
		return resultMod(funcCall);
		break;
	case sign_f:
		return resultSign(funcCall);
		break;
	case abs_f:
		return resultAbs(funcCall);
		break;
	default:
		throw EquationException("Unrecognized function", funcCall->index);
	}
	return 0;
}

double resultPow(AST *powFunc)
{
	AST_list params = powFunc->data.func_call.parameters;

	AST *paramOne = ast_list_first(params);
	double base = resultExpr(paramOne);

	params = ast_list_rest(params);

	AST *paramTwo = ast_list_first(params);
	double power = resultExpr(paramTwo);

	double powResult = pow(base, power);

	return powResult;
}
double resultSin(AST *sinFunc)
{
	AST *param = ast_list_first(sinFunc->data.func_call.parameters);
	double exprResult = resultExpr(param);

	return sin(exprResult);
}

double resultMod(AST *modFunc)
{

	AST_list params = modFunc->data.func_call.parameters;

	AST *paramOne = ast_list_first(params);
	double one = resultExpr(paramOne);

	params = ast_list_rest(params);

	AST *paramTwo = ast_list_first(params);
	double two = resultExpr(paramTwo);

	double modResult = fmod(one, two);

	return modResult;
}

double resultSign(AST *signFunc)
{
	AST *param = ast_list_first(signFunc->data.func_call.parameters);
	double exprResult = resultExpr(param);

	if (exprResult < 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

double resultAbs(AST *absFunc)
{
	AST *param = ast_list_first(absFunc->data.func_call.parameters);
	double exprResult = resultExpr(param);

	return fabs(exprResult);
}

double resultIdent(AST *ident)
{
	string str(ident->data.ident.name);
	if (str.compare("x") == 0)
	{
		return xValue;
	}
	else if (str.compare("f") == 0)
	{
		return fValue;
	}
	else
	{
		throw EquationException("Unknown variable: " + str, ident->index);
	}
}

double resultNum(AST *num)
{
	return num->data.number.value;
}