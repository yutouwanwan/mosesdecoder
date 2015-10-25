/*
 * SearchNormal.cpp
 *
 *  Created on: 25 Oct 2015
 *      Author: hieu
 */

#include "SearchNormal.h"
#include "Stack.h"
#include "Manager.h"
#include "InputPaths.h"
#include "moses/WordsBitmap.h"

SearchNormal::SearchNormal(const Manager &mgr, std::vector<Stack> &stacks)
:m_mgr(mgr)
,m_stacks(stacks)
{
	// TODO Auto-generated constructor stub

}

SearchNormal::~SearchNormal() {
	// TODO Auto-generated destructor stub
}

void SearchNormal::Decode(size_t stackInd)
{
	Stack &stack = m_stacks[stackInd];

	Stack::const_iterator iter;
	for (iter = stack.begin(); iter != stack.end(); ++iter) {
		const Hypothesis &hypo = **iter;
		Extend(hypo);
	}
}

void SearchNormal::Extend(const Hypothesis &hypo)
{
	const InputPaths &paths = m_mgr.GetInputPaths();
	InputPaths::const_iterator iter;
	for (iter = paths.begin(); iter != paths.end(); ++iter) {
		const InputPath &path = *iter;
		Extend(hypo, path);
	}
}

void SearchNormal::Extend(const Hypothesis &hypo, const InputPath &path)
{
	const Moses::WordsBitmap &bitmap = hypo.GetBitmap();
	const Moses::WordsRange &hypoRange = hypo.GetRange();
	const Moses::WordsRange &pathRange = path.GetRange();

	if (bitmap.Overlap(pathRange)) {
		return;
	}

	int distortion = abs((int)pathRange.GetStartPos() - (int)hypoRange.GetEndPos() - 1);
	if (distortion > 5) {

	}
}