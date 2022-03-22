#include <iostream>
#include <string>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <new>
#include <cstring>
#include <cstdint>
#include <cstddef>
#include <cmath>

#define self(ClassMember, Initialization) std::decay_t<decltype(Initialization)> ClassMember = Initialization
#define Begin begin
#define End end

constexpr auto operator""_size(unsigned long long Value) {
	return static_cast<std::size_t>(Value);
}

constexpr auto operator""_ptrdiff(unsigned long long Value) {
	return static_cast<std::ptrdiff_t>(Value);
}

class Range final {
	struct Iterator final {
		self(Cursor, 0_ptrdiff);
		self(Step, 0_ptrdiff);
		Iterator() = default;
		Iterator(std::ptrdiff_t Cursor, std::ptrdiff_t Step) {
			this->Cursor = Cursor;
			this->Step = Step;
		}
		Iterator(Iterator &&) = default;
		Iterator(const Iterator &) = default;
		auto operator=(Iterator &&)->decltype(*this) = default;
		auto operator=(const Iterator &)->decltype(*this) = default;
		~Iterator() = default;
		auto operator*() const {
			return Cursor;
		}
		auto &operator++() {
			Cursor += Step;
			return *this;
		}
		friend auto operator!=(Iterator IteratorA, Iterator IteratorB) {
			if (IteratorA.Step > 0)
				return IteratorA.Cursor < IteratorB.Cursor;
			else if (IteratorA.Step < 0)
				return IteratorA.Cursor > IteratorB.Cursor;
			else
				return IteratorA.Cursor != IteratorB.Cursor;
		}
	};
	self(Startpoint, 0_ptrdiff);
	self(Endpoint, 0_ptrdiff);
	self(Step, 1_ptrdiff);
public:
	Range() = default;
	Range(std::ptrdiff_t Endpoint) {
		if (Endpoint < 0)
			Step = -1;
		this->Endpoint = Endpoint;
	}
	Range(std::ptrdiff_t Startpoint, std::ptrdiff_t Endpoint) {
		if (Startpoint > Endpoint)
			Step = -1;
		this->Startpoint = Startpoint;
		this->Endpoint = Endpoint;
	}
	Range(std::ptrdiff_t Startpoint, std::ptrdiff_t Endpoint, std::ptrdiff_t Step) {
		this->Startpoint = Startpoint;
		this->Endpoint = Endpoint;
		this->Step = Step;
	}
	Range(Range &&) = default;
	Range(const Range &) = default;
	auto operator=(Range &&)->decltype(*this) = default;
	auto operator=(const Range &)->decltype(*this) = default;
	~Range() = default;
	auto Begin() const {
		return Iterator{ Startpoint, Step };
	}
	auto End() const {
		return Iterator{ Endpoint, Step };
	}
};

class BigUnsignedInteger final {
	self(BlockCount, 0_ptrdiff);
	self(DataBlocks, static_cast<std::ptrdiff_t *>(nullptr));
	static constexpr auto WordWidth = sizeof(std::ptrdiff_t) << 3;
	static constexpr auto NumericalWidth = WordWidth - 1;
	static constexpr auto NumericalBitMask = (1_size << NumericalWidth) - 1;
	static constexpr auto MaximumUnsignedWord = static_cast<std::size_t>(-1);
public:
	BigUnsignedInteger() = default;
	auto &operator=(BigUnsignedInteger &&OtherBigUnsignedInteger) {
		if (this != &OtherBigUnsignedInteger) {
			BlockCount = OtherBigUnsignedInteger.BlockCount;
			std::swap(DataBlocks, OtherBigUnsignedInteger.DataBlocks);
		}
		return *this;
	}
	BigUnsignedInteger(BigUnsignedInteger &&OtherBigUnsignedInteger) {
		*this = std::move(OtherBigUnsignedInteger);
	}
	auto &operator=(const BigUnsignedInteger &OtherBigUnsignedInteger) {
		auto ReplicateDataBlocks = [&]() {
			auto DuplicateBlocks = new std::ptrdiff_t[OtherBigUnsignedInteger.BlockCount];
			std::memcpy(DuplicateBlocks, OtherBigUnsignedInteger.DataBlocks, OtherBigUnsignedInteger.BlockCount * sizeof(std::ptrdiff_t));
			delete[] DataBlocks;
			DataBlocks = DuplicateBlocks;
		};
		if (this != &OtherBigUnsignedInteger) {
			BlockCount = OtherBigUnsignedInteger.BlockCount;
			ReplicateDataBlocks();
		}
		return *this;
	}
	BigUnsignedInteger(const BigUnsignedInteger &OtherBigUnsignedInteger) {
		*this = OtherBigUnsignedInteger;
	}
	~BigUnsignedInteger() {
		delete[] DataBlocks;
		BlockCount = 0;
		DataBlocks = nullptr;
	}
	auto &operator+=(BigUnsignedInteger &&RightHandSideOperand) {
		auto Result = BigUnsignedInteger{};
		auto CarryBit = 0;
		auto DoAlignedAddition = [&](auto &&LongerInteger, auto &ShorterInteger) {
			auto AlignedBlockCount = LongerInteger.BlockCount;
			auto AlignedBuffer = new std::ptrdiff_t[AlignedBlockCount];
			auto InitializeOperands = [&]() {
				std::memset(AlignedBuffer, 0, AlignedBlockCount * sizeof(std::ptrdiff_t));
				std::memcpy(AlignedBuffer, ShorterInteger.DataBlocks, ShorterInteger.BlockCount * sizeof(std::ptrdiff_t));
				Result = std::move(LongerInteger);
			};
			auto DoBlockWiseAddition = [&](auto BlockIndex) {
				Result.DataBlocks[BlockIndex] += AlignedBuffer[BlockIndex] + CarryBit;
				CarryBit = Result.DataBlocks[BlockIndex] < 0;
				Result.DataBlocks[BlockIndex] &= NumericalBitMask;
			};
			InitializeOperands();
			for (auto x : Range{ AlignedBlockCount })
				DoBlockWiseAddition(x);
			delete[] AlignedBuffer;
		};
		auto AttachMSBToDataBlocks = [&]() {
			auto CalibratedDataBlocks = new std::ptrdiff_t[Result.BlockCount + 1];
			std::memcpy(CalibratedDataBlocks, Result.DataBlocks, Result.BlockCount * sizeof(std::ptrdiff_t));
			CalibratedDataBlocks[Result.BlockCount] = 1;
			Result.BlockCount += 1;
			delete[] Result.DataBlocks;
			Result.DataBlocks = CalibratedDataBlocks;
		};
		if (BlockCount > RightHandSideOperand.BlockCount)
			DoAlignedAddition(std::move(*this), RightHandSideOperand);
		else
			DoAlignedAddition(std::move(RightHandSideOperand), *this);
		if (CarryBit == 1)
			AttachMSBToDataBlocks();
		*this = std::move(Result);
		return *this;
	}
	auto &operator-=(const BigUnsignedInteger &RightHandSideOperand) {
		auto Result = BigUnsignedInteger{};
		auto DoAlignedSubtraction = [&]() {
			auto AlignedBlockCount = BlockCount;
			auto ValidBlockCount = std::min(AlignedBlockCount, RightHandSideOperand.BlockCount);
			auto AlignedBuffer = new std::ptrdiff_t[AlignedBlockCount];
			auto BorrowBit = 0;
			auto InitializeOperands = [&]() {
				std::memset(AlignedBuffer, 0, AlignedBlockCount * sizeof(std::ptrdiff_t));
				std::memcpy(AlignedBuffer, RightHandSideOperand.DataBlocks, ValidBlockCount * sizeof(std::ptrdiff_t));
				Result = std::move(*this);
			};
			auto DoBlockWiseSubtraction = [&](auto BlockIndex) {
				Result.DataBlocks[BlockIndex] -= AlignedBuffer[BlockIndex] + BorrowBit;
				BorrowBit = Result.DataBlocks[BlockIndex] < 0;
				Result.DataBlocks[BlockIndex] &= NumericalBitMask;
			};
			InitializeOperands();
			for (auto x : Range{ AlignedBlockCount })
				DoBlockWiseSubtraction(x);
			delete[] AlignedBuffer;
		};
		auto ZapOffZeroMSBFromDataBlocks = [&]() {
			auto GetValidBlockCount = [&]() {
				for (auto x : Range{ Result.BlockCount })
					if (Result.DataBlocks[Result.BlockCount - 1 - x] == 0)
						continue;
					else
						return Result.BlockCount - x;
				return 0_ptrdiff;
			};
			auto ValidBlockCount = GetValidBlockCount();
			auto DecapitateDataBlocks = [&]() {
				auto CalibratedDataBlocks = new std::ptrdiff_t[ValidBlockCount];
				std::memcpy(CalibratedDataBlocks, Result.DataBlocks, ValidBlockCount * sizeof(std::ptrdiff_t));
				Result.BlockCount = ValidBlockCount;
				delete[] Result.DataBlocks;
				Result.DataBlocks = CalibratedDataBlocks;
			};
			if (Result.BlockCount != ValidBlockCount)
				DecapitateDataBlocks();
		};
		DoAlignedSubtraction();
		ZapOffZeroMSBFromDataBlocks();
		*this = std::move(Result);
		return *this;
	}
	BigUnsignedInteger(unsigned long long Value) {
		constexpr auto Threshold = static_cast<std::ptrdiff_t>((1ull << NumericalWidth) - 1);
		auto GeneratePrimitiveBigUnsignedInteger = [](auto Value) {
			auto PrimitiveBigUnsignedInteger = BigUnsignedInteger{};
			PrimitiveBigUnsignedInteger.BlockCount = 1;
			PrimitiveBigUnsignedInteger.DataBlocks = new std::ptrdiff_t[PrimitiveBigUnsignedInteger.BlockCount];
			PrimitiveBigUnsignedInteger.DataBlocks[0] = Value;
			return PrimitiveBigUnsignedInteger;
		};
		auto DoPartialConstruction = [&]() {
			*this += GeneratePrimitiveBigUnsignedInteger(Threshold);
			Value -= Threshold;
		};
		while (Value > Threshold)
			DoPartialConstruction();
		if (Value != 0)
			*this += GeneratePrimitiveBigUnsignedInteger(static_cast<std::ptrdiff_t>(Value));
	}
	friend auto operator==(const BigUnsignedInteger &BigUnsignedIntegerA, const BigUnsignedInteger &BigUnsignedIntegerB) {
		auto DoBlockWiseCompare = [&]() {
			for (auto x : Range{ BigUnsignedIntegerA.BlockCount })
				if (BigUnsignedIntegerA.DataBlocks[x] == BigUnsignedIntegerB.DataBlocks[x])
					continue;
				else
					return false;
			return true;
		};
		if (BigUnsignedIntegerA.BlockCount != BigUnsignedIntegerB.BlockCount)
			return false;
		else
			return DoBlockWiseCompare();
	}
	friend auto operator<(const BigUnsignedInteger &BigUnsignedIntegerA, const BigUnsignedInteger &BigUnsignedIntegerB) {
		auto DoBlockWiseCompare = [&]() {
			for (auto x : Range{ BigUnsignedIntegerA.BlockCount })
				if (BigUnsignedIntegerA.DataBlocks[BigUnsignedIntegerA.BlockCount - 1 - x] == BigUnsignedIntegerB.DataBlocks[BigUnsignedIntegerB.BlockCount - 1 - x])
					continue;
				else if (BigUnsignedIntegerA.DataBlocks[BigUnsignedIntegerA.BlockCount - 1 - x] < BigUnsignedIntegerB.DataBlocks[BigUnsignedIntegerB.BlockCount - 1 - x])
					return true;
				else
					return false;
			return false;
		};
		if (BigUnsignedIntegerA.BlockCount < BigUnsignedIntegerB.BlockCount)
			return true;
		else if (BigUnsignedIntegerA.BlockCount > BigUnsignedIntegerB.BlockCount)
			return false;
		else
			return DoBlockWiseCompare();
	}
	friend auto operator>(const BigUnsignedInteger &BigUnsignedIntegerA, const BigUnsignedInteger &BigUnsignedIntegerB) {
		auto DoBlockWiseCompare = [&]() {
			for (auto x : Range{ BigUnsignedIntegerA.BlockCount })
				if (BigUnsignedIntegerA.DataBlocks[BigUnsignedIntegerA.BlockCount - 1 - x] == BigUnsignedIntegerB.DataBlocks[BigUnsignedIntegerB.BlockCount - 1 - x])
					continue;
				else if (BigUnsignedIntegerA.DataBlocks[BigUnsignedIntegerA.BlockCount - 1 - x] > BigUnsignedIntegerB.DataBlocks[BigUnsignedIntegerB.BlockCount - 1 - x])
					return true;
				else
					return false;
			return false;
		};
		if (BigUnsignedIntegerA.BlockCount > BigUnsignedIntegerB.BlockCount)
			return true;
		else if (BigUnsignedIntegerA.BlockCount < BigUnsignedIntegerB.BlockCount)
			return false;
		else
			return DoBlockWiseCompare();
	}
	explicit operator unsigned long long() const {
		constexpr auto UnsignedLongLongWidth = sizeof(unsigned long long) << 3;
		constexpr auto ResidualBitCount = UnsignedLongLongWidth % NumericalWidth;
		constexpr auto CompleteBlockCount = UnsignedLongLongWidth / NumericalWidth;
		constexpr auto UsableBlockCount = ResidualBitCount == 0 ? CompleteBlockCount : CompleteBlockCount + 1;
		auto TruncatedResult = 0ull;
		for (auto x : Range{ UsableBlockCount - 1, -1 })
			if (BlockCount > x)
				TruncatedResult = (TruncatedResult << NumericalWidth) + DataBlocks[x];
		return TruncatedResult;
	}
	auto &operator<<=(BigUnsignedInteger &&RightHandSideOperand) {
		auto DoNativeBitShift = [&](auto PartialShift) {
			auto ResidualBitShift = PartialShift % NumericalWidth;
			auto BlockShift = static_cast<decltype(BlockCount)>(PartialShift / NumericalWidth);
			auto DoResidualBitShift = [&]() {
				auto CarryBits = 0_ptrdiff;
				auto DoBlockWiseBitShift = [&](auto BlockIndex) {
					auto BlockReplicate = DataBlocks[BlockIndex];
					DataBlocks[BlockIndex] <<= ResidualBitShift;
					DataBlocks[BlockIndex] &= NumericalBitMask;
					DataBlocks[BlockIndex] += CarryBits;
					CarryBits = BlockReplicate >> (NumericalWidth - ResidualBitShift);
				};
				auto AttachMSBToDataBlocks = [&]() {
					auto CalibratedDataBlocks = new std::ptrdiff_t[BlockCount + 1];
					std::memcpy(CalibratedDataBlocks, DataBlocks, BlockCount * sizeof(std::ptrdiff_t));
					CalibratedDataBlocks[BlockCount] = CarryBits;
					BlockCount += 1;
					delete[] DataBlocks;
					DataBlocks = CalibratedDataBlocks;
				};
				for (auto x : Range{ BlockCount })
					DoBlockWiseBitShift(x);
				if (CarryBits != 0)
					AttachMSBToDataBlocks();
			};
			auto DoBlockShift = [&]() {
				auto ShiftedBlockCount = BlockCount + BlockShift;
				auto ShiftedDataBlocks = new std::ptrdiff_t[ShiftedBlockCount];
				std::memset(ShiftedDataBlocks, 0, ShiftedBlockCount * sizeof(std::ptrdiff_t));
				std::memcpy(ShiftedDataBlocks + BlockShift, DataBlocks, BlockCount * sizeof(std::ptrdiff_t));
				BlockCount = ShiftedBlockCount;
				delete[] DataBlocks;
				DataBlocks = ShiftedDataBlocks;
			};
			if (ResidualBitShift != 0)
				DoResidualBitShift();
			if (BlockCount > 0 && BlockShift != 0)
				DoBlockShift();
		};
		while (RightHandSideOperand > MaximumUnsignedWord) {
			DoNativeBitShift(MaximumUnsignedWord);
			RightHandSideOperand -= MaximumUnsignedWord;
		}
		DoNativeBitShift(static_cast<unsigned long long>(RightHandSideOperand));
		return *this;
	}
	auto &operator>>=(BigUnsignedInteger &&RightHandSideOperand) {
		auto DoNativeBitShift = [&](auto PartialShift) {
			auto ResidualBitShift = PartialShift % NumericalWidth;
			auto BlockShift = static_cast<decltype(BlockCount)>(PartialShift / NumericalWidth);
			auto DoResidualBitShift = [&]() {
				auto BorrowBits = 0_ptrdiff;
				auto DoBlockWiseBitShift = [&](auto BlockIndex) {
					auto BlockReplicate = DataBlocks[BlockIndex];
					DataBlocks[BlockIndex] >>= ResidualBitShift;
					DataBlocks[BlockIndex] += BorrowBits;
					BorrowBits = BlockReplicate << (NumericalWidth - ResidualBitShift);
					BorrowBits &= NumericalBitMask;
				};
				auto ZapOffZeroMSBFromDataBlocks = [&]() {
					auto CalibratedDataBlocks = new std::ptrdiff_t[BlockCount - 1];
					BlockCount -= 1;
					std::memcpy(CalibratedDataBlocks, DataBlocks, BlockCount * sizeof(std::ptrdiff_t));
					delete[] DataBlocks;
					DataBlocks = CalibratedDataBlocks;
				};
				for (auto x : Range{ BlockCount - 1, -1 })
					DoBlockWiseBitShift(x);
				if (BlockCount > 0 && DataBlocks[BlockCount - 1] == 0)
					ZapOffZeroMSBFromDataBlocks();
			};
			auto DoBlockShift = [&]() {
				auto ShiftedBlockCount = BlockCount - BlockShift;
				auto ShiftedDataBlocks = new std::ptrdiff_t[ShiftedBlockCount];
				std::memcpy(ShiftedDataBlocks, DataBlocks + BlockShift, ShiftedBlockCount * sizeof(std::ptrdiff_t));
				BlockCount = ShiftedBlockCount;
				delete[] DataBlocks;
				DataBlocks = ShiftedDataBlocks;
			};
			if (ResidualBitShift != 0)
				DoResidualBitShift();
			if (BlockCount > BlockShift && BlockShift != 0)
				DoBlockShift();
			else if (BlockCount <= BlockShift)
				this->~BigUnsignedInteger();
		};
		while (RightHandSideOperand > MaximumUnsignedWord) {
			DoNativeBitShift(MaximumUnsignedWord);
			RightHandSideOperand -= MaximumUnsignedWord;
		}
		DoNativeBitShift(static_cast<unsigned long long>(RightHandSideOperand));
		return *this;
	}
	explicit operator long double() {
		auto Result = 0.l;
		auto CalibrationFactor = BigUnsignedInteger{};
		auto ExtractOneBinaryBit = [&]() {
			Result += DataBlocks[0] & 0x1;
			Result /= 2;
			CalibrationFactor += 1;
			*this >>= 1;
		};
		auto CalibrateTheResult = [&]() {
			Result *= 2;
			CalibrationFactor -= 1;
		};
		while (*this > 0)
			ExtractOneBinaryBit();
		while (CalibrationFactor > 0)
			CalibrateTheResult();
		return Result;
	}
	auto &operator*=(BigUnsignedInteger &&RightHandSideOperand) {
		auto Result = BigUnsignedInteger{};
		auto CurrentShift = BigUnsignedInteger{};
		auto DoBinaryConvolution = [&]() {
			auto ApplyPartialConvolution = [&]() {
				auto LeftHandSideOperandReplicate = *this;
				LeftHandSideOperandReplicate <<= BigUnsignedInteger{ CurrentShift };
				Result += std::move(LeftHandSideOperandReplicate);
			};
			if (auto ConvolutionWeight = RightHandSideOperand.DataBlocks[0] & 0x1; ConvolutionWeight == 1)
				ApplyPartialConvolution();
			CurrentShift += 1;
			RightHandSideOperand >>= 1;
		};
		while (RightHandSideOperand > 0)
			DoBinaryConvolution();
		*this = std::move(Result);
		return *this;
	}
private:
	auto DoInternalDivision(const auto &RightHandSideOperand, auto QuotientPointer, auto RemainderPointer) {
		auto ReversedLeftHandSideOperand = BigUnsignedInteger{};
		auto LeftHandSideOperandTailingZeros = BigUnsignedInteger{};
		auto Quotient = BigUnsignedInteger{};
		auto Remainder = BigUnsignedInteger{};
		auto PreprocessLeftHandSideOperand = [&]() {
			auto GetTailingZeros = [&]() {
				auto TailingBit = DataBlocks[0] & 0x1;
				auto RightShiftAndCheckAgain = [&]() {
					LeftHandSideOperandTailingZeros += 1;
					*this >>= 1;
					TailingBit = DataBlocks[0] & 0x1;
				};
				while (TailingBit == 0)
					RightShiftAndCheckAgain();
			};
			auto ReverseTheRest = [&]() {
				auto KeepReversing = [&]() {
					ReversedLeftHandSideOperand <<= 1;
					ReversedLeftHandSideOperand += DataBlocks[0] & 0x1;
					*this >>= 1;
				};
				while (*this > 0)
					KeepReversing();
			};
			if (BlockCount > 0)
				GetTailingZeros();
			ReverseTheRest();
		};
		auto DoPartialDivision = [&](auto &&DealWithTheTailingBitOfTheRemainder) {
			auto CalibrateTheRemainderAndQuotient = [&]() {
				Remainder -= RightHandSideOperand;
				Quotient += 1;
			};
			Remainder <<= 1;
			DealWithTheTailingBitOfTheRemainder();
			Quotient <<= 1;
			if (Remainder < RightHandSideOperand == false)
				CalibrateTheRemainderAndQuotient();
		};
		PreprocessLeftHandSideOperand();
		while (ReversedLeftHandSideOperand > 0)
			DoPartialDivision([&]() {
				Remainder += ReversedLeftHandSideOperand.DataBlocks[0] & 0x1;
				ReversedLeftHandSideOperand >>= 1;
			});
		while (LeftHandSideOperandTailingZeros > 0)
			DoPartialDivision([&]() {
				LeftHandSideOperandTailingZeros -= 1;
			});
		if (QuotientPointer != nullptr)
			*QuotientPointer = std::move(Quotient);
		if (RemainderPointer != nullptr)
			*RemainderPointer = std::move(Remainder);
	}
public:
	auto &operator/=(const BigUnsignedInteger &RightHandSideOperand) {
		DoInternalDivision(RightHandSideOperand, this, static_cast<decltype(this)>(nullptr));
		return *this;
	}
	auto &operator%=(const BigUnsignedInteger &RightHandSideOperand) {
		DoInternalDivision(RightHandSideOperand, static_cast<decltype(this)>(nullptr), this);
		return *this;
	}
	BigUnsignedInteger(std::string &&Value) {
		auto ExtractOneDecimalDigit = [&]() {
			*this *= 10;
			*this += Value[0] - '0';
			Value.erase(Value.begin());
		};
		while (Value.size() > 0)
			ExtractOneDecimalDigit();
	}
	explicit operator std::string() {
		using namespace std::literals;
		auto DecimalRepresentation = ""s;
		auto ExtractOneDecimalDigit = [&]() {
			auto DecimalDigit = BigUnsignedInteger{};
			DoInternalDivision(10, this, &DecimalDigit);
			DecimalRepresentation += static_cast<char>(static_cast<unsigned long long>(DecimalDigit) + '0');
		};
		if (*this == 0)
			DecimalRepresentation = "0";
		while (*this > 0)
			ExtractOneDecimalDigit();
		std::reverse(DecimalRepresentation.begin(), DecimalRepresentation.end());
		return DecimalRepresentation;
	}
	friend auto &operator<<(std::ostream &Output, BigUnsignedInteger &&SomeBigUnsignedInteger) {
		Output << static_cast<std::string>(SomeBigUnsignedInteger);
		return Output;
	}
};

class BigInteger final {
	static constexpr auto Positive = false;
	static constexpr auto Negative = true;
	self(Sign, Positive);
	self(Magnitude, BigUnsignedInteger{});
public:
	BigInteger() = default;
	BigInteger(unsigned long long Value) {
		Magnitude = Value;
	}
	BigInteger(long long Value) {
		if (Value < 0) {
			Value = -Value;
			Sign = Negative;
		}
		Magnitude = static_cast<unsigned long long>(Value);
	}
	BigInteger(unsigned long Value) :BigInteger{ static_cast<unsigned long long>(Value) } {}
	BigInteger(long Value) :BigInteger{ static_cast<long long>(Value) } {}
	BigInteger(unsigned int Value) :BigInteger{ static_cast<unsigned long long>(Value) } {}
	BigInteger(int Value) :BigInteger{ static_cast<long long>(Value) } {}
	BigInteger(std::string &&Value) {
		auto GetValueSign = [&]() {
			if (Value.size() > 0)
				if (Value[0] == '-')
					Sign = Negative;
		};
		auto StripAwayTheSignCharacter = [&]() {
			if (Value.size() > 0)
				if (Value[0] == '+' || Value[0] == '-')
					Value.erase(Value.begin());
		};
		GetValueSign();
		StripAwayTheSignCharacter();
		Magnitude = std::move(Value);
	}
	BigInteger(const std::string &Value) :BigInteger{ std::string{ Value } } {}
	BigInteger(const BigInteger &OtherBigInteger) = default;
	auto operator=(const BigInteger &OtherBigInteger)->decltype(*this) = default;
	auto &operator=(BigInteger &&OtherBigInteger) {
		if (this != &OtherBigInteger) {
			Sign = OtherBigInteger.Sign;
			Magnitude = std::move(OtherBigInteger.Magnitude);
		}
		return *this;
	}
	BigInteger(BigInteger &&OtherBigInteger) {
		*this = std::move(OtherBigInteger);
	}
	~BigInteger() = default;
	auto &operator+=(BigInteger &&RightHandSideOperand) {
		auto ReverseTheSignAndCalibrateTheMagnitude = [&]() {
			auto CurrentMagnitude = std::move(Magnitude);
			Magnitude = std::move(RightHandSideOperand.Magnitude);
			Magnitude -= CurrentMagnitude;
			Sign = RightHandSideOperand.Sign;
		};
		if (RightHandSideOperand.Sign == Sign)
			Magnitude += std::move(RightHandSideOperand.Magnitude);
		else if (Magnitude > RightHandSideOperand.Magnitude)
			Magnitude -= RightHandSideOperand.Magnitude;
		else
			ReverseTheSignAndCalibrateTheMagnitude();
		return *this;
	}
	auto &operator+=(const BigInteger &RightHandSideOperand) {
		*this += BigInteger{ RightHandSideOperand };
		return *this;
	}
	friend auto operator+(BigInteger &&Operand) {
		return std::move(Operand);
	}
	friend auto operator+(const BigInteger &Operand) {
		return Operand;
	}
	friend auto operator-(BigInteger &&Operand) {
		Operand.Sign ^= 1;
		return std::move(Operand);
	}
	friend auto operator-(const BigInteger &Operand) {
		return -BigInteger{ Operand };
	}
	auto &operator-=(BigInteger &&RightHandSideOperand) {
		*this += -std::move(RightHandSideOperand);
		return *this;
	}
	auto &operator-=(const BigInteger &RightHandSideOperand) {
		*this -= BigInteger{ RightHandSideOperand };
		return *this;
	}
	friend auto operator==(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		if (BigIntegerA.Magnitude == 0)
			return BigIntegerB.Magnitude == 0;
		else
			return BigIntegerA.Sign == BigIntegerB.Sign && BigIntegerA.Magnitude == BigIntegerB.Magnitude;
	}
	friend auto operator!=(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		return !(BigIntegerA == BigIntegerB);
	}
	friend auto operator<(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		if (BigIntegerA.Magnitude == 0 && BigIntegerB.Magnitude == 0)
			return false;
		else if (BigIntegerA.Sign == Negative)
			if (BigIntegerB.Sign == Positive)
				return true;
			else
				return BigIntegerA.Magnitude > BigIntegerB.Magnitude;
		else
			if (BigIntegerB.Sign == Negative)
				return false;
			else
				return BigIntegerA.Magnitude < BigIntegerB.Magnitude;
	}
	friend auto operator>(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		if (BigIntegerA.Magnitude == 0 && BigIntegerB.Magnitude == 0)
			return false;
		else if (BigIntegerA.Sign == Positive)
			if (BigIntegerB.Sign == Negative)
				return true;
			else
				return BigIntegerA.Magnitude > BigIntegerB.Magnitude;
		else
			if (BigIntegerB.Sign == Positive)
				return false;
			else
				return BigIntegerA.Magnitude < BigIntegerB.Magnitude;
	}
	friend auto operator<=(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		return !(BigIntegerA > BigIntegerB);
	}
	friend auto operator>=(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		return !(BigIntegerA < BigIntegerB);
	}
	auto &operator<<=(BigInteger &&RightHandSideOperand) {
		if (RightHandSideOperand.Sign == Positive)
			Magnitude <<= std::move(RightHandSideOperand.Magnitude);
		else
			Magnitude >>= std::move(RightHandSideOperand.Magnitude);
		return *this;
	}
	auto &operator<<=(const BigInteger &RightHandSideOperand) {
		*this <<= BigInteger{ RightHandSideOperand };
		return *this;
	}
	auto &operator>>=(BigInteger &&RightHandSideOperand) {
		if (RightHandSideOperand.Sign == Positive)
			Magnitude >>= std::move(RightHandSideOperand.Magnitude);
		else
			Magnitude <<= std::move(RightHandSideOperand.Magnitude);
		return *this;
	}
	auto &operator>>=(const BigInteger &RightHandSideOperand) {
		*this >>= BigInteger{ RightHandSideOperand };
		return *this;
	}
	auto &operator*=(BigInteger &&RightHandSideOperand) {
		Sign ^= RightHandSideOperand.Sign;
		Magnitude *= std::move(RightHandSideOperand.Magnitude);
		return *this;
	}
	auto &operator*=(const BigInteger &RightHandSideOperand) {
		*this *= BigInteger{ RightHandSideOperand };
		return *this;
	}
	auto &operator/=(const BigInteger &RightHandSideOperand) {
		Sign ^= RightHandSideOperand.Sign;
		Magnitude /= RightHandSideOperand.Magnitude;
		return *this;
	}
	auto &operator%=(const BigInteger &RightHandSideOperand) {
		Magnitude %= RightHandSideOperand.Magnitude;
		return *this;
	}
	auto &operator++() {
		*this += 1;
		return *this;
	}
	auto operator++(int) {
		auto SelfReplicate = *this;
		*this += 1;
		return SelfReplicate;
	}
	auto &operator--() {
		*this -= 1;
		return *this;
	}
	auto operator--(int) {
		auto SelfReplicate = *this;
		*this -= 1;
		return SelfReplicate;
	}
	friend auto operator+(BigInteger &&BigIntegerA, BigInteger &&BigIntegerB) {
		BigIntegerA += std::move(BigIntegerB);
		return std::move(BigIntegerA);
	}
	friend auto operator+(BigInteger &&BigIntegerA, const BigInteger &BigIntegerB) {
		BigIntegerA += BigIntegerB;
		return std::move(BigIntegerA);
	}
	friend auto operator+(const BigInteger &BigIntegerA, BigInteger &&BigIntegerB) {
		BigIntegerB += BigIntegerA;
		return std::move(BigIntegerB);
	}
	friend auto operator+(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		return BigInteger{ BigIntegerA } + BigIntegerB;
	}
	friend auto operator-(BigInteger &&BigIntegerA, BigInteger &&BigIntegerB) {
		BigIntegerA -= std::move(BigIntegerB);
		return std::move(BigIntegerA);
	}
	friend auto operator-(BigInteger &&BigIntegerA, const BigInteger &BigIntegerB) {
		BigIntegerA -= BigIntegerB;
		return std::move(BigIntegerA);
	}
	friend auto operator-(const BigInteger &BigIntegerA, BigInteger &&BigIntegerB) {
		return BigInteger{ BigIntegerA } - std::move(BigIntegerB);
	}
	friend auto operator-(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		return BigInteger{ BigIntegerA } - BigIntegerB;
	}
	friend auto operator<<(BigInteger &&BigIntegerA, BigInteger &&BigIntegerB) {
		BigIntegerA <<= std::move(BigIntegerB);
		return std::move(BigIntegerA);
	}
	friend auto operator<<(BigInteger &&BigIntegerA, const BigInteger &BigIntegerB) {
		BigIntegerA <<= BigIntegerB;
		return std::move(BigIntegerA);
	}
	friend auto operator<<(const BigInteger &BigIntegerA, BigInteger &&BigIntegerB) {
		return BigInteger{ BigIntegerA } << std::move(BigIntegerB);
	}
	friend auto operator<<(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		return BigInteger{ BigIntegerA } << BigIntegerB;
	}
	friend auto operator>>(BigInteger &&BigIntegerA, BigInteger &&BigIntegerB) {
		BigIntegerA >>= std::move(BigIntegerB);
		return std::move(BigIntegerA);
	}
	friend auto operator>>(BigInteger &&BigIntegerA, const BigInteger &BigIntegerB) {
		BigIntegerA >>= BigIntegerB;
		return std::move(BigIntegerA);
	}
	friend auto operator>>(const BigInteger &BigIntegerA, BigInteger &&BigIntegerB) {
		return BigInteger{ BigIntegerA } >> std::move(BigIntegerB);
	}
	friend auto operator>>(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		return BigInteger{ BigIntegerA } >> BigIntegerB;
	}
	friend auto operator*(BigInteger &&BigIntegerA, BigInteger &&BigIntegerB) {
		BigIntegerA *= std::move(BigIntegerB);
		return std::move(BigIntegerA);
	}
	friend auto operator*(BigInteger &&BigIntegerA, const BigInteger &BigIntegerB) {
		BigIntegerA *= BigIntegerB;
		return std::move(BigIntegerA);
	}
	friend auto operator*(const BigInteger &BigIntegerA, BigInteger &&BigIntegerB) {
		BigIntegerB *= BigIntegerA;
		return std::move(BigIntegerB);
	}
	friend auto operator*(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		return BigInteger{ BigIntegerA } * BigIntegerB;
	}
	friend auto operator/(BigInteger &&BigIntegerA, const BigInteger &BigIntegerB) {
		BigIntegerA /= BigIntegerB;
		return std::move(BigIntegerA);
	}
	friend auto operator/(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		return BigInteger{ BigIntegerA } / BigIntegerB;
	}
	friend auto operator%(BigInteger &&BigIntegerA, const BigInteger &BigIntegerB) {
		BigIntegerA %= BigIntegerB;
		return std::move(BigIntegerA);
	}
	friend auto operator%(const BigInteger &BigIntegerA, const BigInteger &BigIntegerB) {
		return BigInteger{ BigIntegerA } % BigIntegerB;
	}
	explicit operator unsigned long long() const {
		auto Result = static_cast<unsigned long long>(Magnitude);
		if (Sign == Negative)
			Result = static_cast<unsigned long long>(-static_cast<long long>(Result));
		return Result;
	}
	explicit operator long long() const {
		auto AbsoluteValue = static_cast<unsigned long long>(Magnitude);
		auto Result = static_cast<long long>(AbsoluteValue);
		if (Sign == Negative)
			Result = -Result;
		return Result;
	}
	explicit operator long double() const {
		auto MagnitudeReplicate = Magnitude;
		auto Result = static_cast<long double>(MagnitudeReplicate);
		if (Sign == Negative)
			Result = -Result;
		return Result;
	}
	explicit operator std::string() const {
		auto MagnitudeReplicate = Magnitude;
		auto DecimalRepresentation = static_cast<std::string>(MagnitudeReplicate);
		if (Sign == Negative && Magnitude > 0)
			DecimalRepresentation = "-" + std::move(DecimalRepresentation);
		return DecimalRepresentation;
	}
	friend auto &operator<<(std::ostream &Output, BigInteger &&SomeBigInteger) {
		if (SomeBigInteger.Sign == Negative && SomeBigInteger.Magnitude > 0)
			Output << "-";
		Output << std::move(SomeBigInteger.Magnitude);
		return Output;
	}
	friend auto &operator<<(std::ostream &Output, const BigInteger &SomeBigInteger) {
		Output << BigInteger{ SomeBigInteger };
		return Output;
	}
	friend auto &operator>>(std::istream &Input, BigInteger &SomeBigInteger) {
		using namespace std::literals;
		auto DataBuffer = ""s;
		Input >> DataBuffer;
		SomeBigInteger.~BigInteger();
		new(&SomeBigInteger) BigInteger{ std::move(DataBuffer) };
		return Input;
	}
};

auto operator""_big(unsigned long long Value) {
	return BigInteger{ Value };
}

auto operator""_big(const char *Value, std::size_t Length) {
	return BigInteger{ std::string{ Value, Length } };
}

auto factorial(auto n) {
    if (n == 0)
        return static_cast<decltype(n)>(1);
    else
        return n * factorial(n - 1);
}

auto main()->int {
	std::cout << factorial(200_big) << std::endl;
}