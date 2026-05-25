# Coding Style Guidelines
Code written for the engine must follow style guideline to keep it uniform and avoid time waste on reading non-standardised code. 

### Engine explicitly disables multiple C++ features, so using them anywhere is impossible.

Disabled features:
 * Exception (try, catch, throw)
 * RTTI (dynamic_cast)

### This guide contains optimization & style guidelines for following sections of code:
 * [General Code](./General.md)
 * Per Frame Code
 * User Oriented Code
 * [Performance Critical Code](./PerformanceCriticalCode.md)
 * Stability Critical Code