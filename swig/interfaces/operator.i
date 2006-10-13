// Operators we don't want to wrap
%ignore operator =;
%ignore operator new;
%ignore operator delete;
%ignore operator <<;

// Operators we do want
%rename(operator_equal_to) operator==;
%rename(operator_not_equal_to) operator!=;

