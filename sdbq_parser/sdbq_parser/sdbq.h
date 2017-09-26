#pragma once
#include "sdbq_decls.h"
#include "sdbq_parser.h"
#include "sdbq_writer.h"

namespace sdbq
{
	enum class ParseErrc
	{
		// no 0
		CannotReadInput = 1,		// input file doest exist or malformed
	};

	enum class MergeErrc
	{
		// no 0
		CannotReadInput = 1,		// input file doest exist or malformed
		CannotCreateOutput = 1<<1,	// cannot create or write to output
	};
	
	struct ParseErrorCategory : std::error_category
	{
		const char* name() const noexcept override;
		std::string message(int ev) const override;
	};

	const char* ParseErrorCategory::name() const noexcept
	{
		return "Parse";
	}

	std::string ParseErrorCategory::message(int ev) const
	{
		switch (static_cast<ParseErrc>(ev))
		{
		case ParseErrc::CannotReadInput:
			return "input file doest exist or malformed";
		default:
			return "(unrecognized error)";
		}
	}

	struct MergeErrorCategory : std::error_category
	{
		const char* name() const noexcept override;
		std::string message(int ev) const override;
	};

	const char* MergeErrorCategory::name() const noexcept
	{
		return "Merge";
	}

	std::string MergeErrorCategory::message(int ev) const
	{
		switch (static_cast<MergeErrc>(ev))
		{
		case MergeErrc::CannotReadInput:
			return "input file doest exist or malformed";
		case MergeErrc::CannotCreateOutput:
			return "cannot create or write to output";
		default:
			return "(unrecognized error)";
		}
	}

	const ParseErrorCategory KTheParseErrorCategory{};
	const MergeErrorCategory KTheMergeErrorCategory{};

		
}
