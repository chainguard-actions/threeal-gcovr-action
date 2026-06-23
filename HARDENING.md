<!-- markdownlint-disable -->

# Hardening Report: threeal--gcovr-action/v0.1.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `1`

Action **threeal--gcovr-action/v0.1.0** was hardened automatically. 15 finding(s) were identified and resolved across 2 iteration(s).

## Findings Fixed

### script-injection (severity: high)

Sub-rule (a): The 'Process inputs' step directly interpolates ${{ inputs.* }} expressions inside the run: shell script. Multiple inputs are interpolated: inputs.root, inputs.gcov-executable, inputs.exclude, inputs.fail-under-line, inputs.coveralls-out, and inputs.coveralls-send. Although they are placed inside single-quoted strings, a single quote in any input value will break out of the quoting and allow arbitrary shell command injection. Example offending lines: `if [ -n '${{ inputs.root }}' ]; then` and `ARGS="$ARGS --root '${{ inputs.root }}'"`. All ${{ inputs.* }} references must be moved to env: variables and then double-quoted in the shell script.

Locations:

- `action.yml:42`

### script-injection (severity: high)

Sub-rule (a): The 'Generate code coverage report using gcovr' step directly interpolates ${{ env.GCOVR_ARGS }} inside the run: shell command: `run: gcovr ${{ env.GCOVR_ARGS }}`. The GCOVR_ARGS value is derived from user-controlled inputs and is injected directly into the shell command string before the shell ever sees it, enabling command injection.

Locations:

- `action.yml:67`

### script-injection (severity: high)

Sub-rule (a): The 'Send code coverage report to Coveralls' step directly interpolates ${{ env.COVERALLS_OUT }} inside the run: shell command: `run: curl -v -F json_file=@${{ env.COVERALLS_OUT }} https://coveralls.io/api/v1/jobs`. The COVERALLS_OUT value is derived from user-controlled inputs.coveralls-out and is injected directly into the shell command string, enabling command injection.

Locations:

- `action.yml:71`

### github-env-injection (severity: high)

The 'Process inputs' step writes values derived from user-controlled inputs to $GITHUB_ENV without sanitization. ARGS is built by concatenating ${{ inputs.root }}, ${{ inputs.gcov-executable }}, ${{ inputs.exclude }}, ${{ inputs.fail-under-line }}, and ${{ inputs.coveralls-out }} directly into the shell variable, then written with `echo "GCOVR_ARGS=$ARGS" >> $GITHUB_ENV`. Similarly, COVERALLS_OUT is set from ${{ inputs.coveralls-out }} and written with `echo "COVERALLS_OUT=$COVERALLS_OUT" >> $GITHUB_ENV`. Neither write is preceded by the required `printf '%s' ... | tr -d '\n\r'` sanitization, allowing newline injection to set arbitrary environment variables.

Locations:

- `action.yml:62`
- `action.yml:63`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.root }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:44`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.root }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:45`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.gcov-executable }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:47`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.gcov-executable }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:48`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.exclude }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:50`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.exclude }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:51`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.fail-under-line }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:53`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.fail-under-line }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:54`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.coveralls-out }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:56`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.coveralls-out }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:57`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.coveralls-send }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:58`

## Iteration Notes

### Iteration 1

**Fixes applied:** script-injection, github-env-injection, static-inline-injection

**Notes:**

Fixed all findings in action.yml: (1) Moved all ${{ inputs.* }} expressions from run: shell scripts to env: blocks in the 'Process inputs' step, using $INPUT_ROOT, $INPUT_GCOV_EXECUTABLE, $INPUT_EXCLUDE, $INPUT_FAIL_UNDER_LINE, $INPUT_COVERALLS_OUT, $INPUT_COVERALLS_SEND env vars in the shell script. (2) Added sanitization using printf '%s' ... | tr -d '\n\r' before writing GCOVR_ARGS and COVERALLS_OUT to $GITHUB_ENV to prevent newline injection. (3) Replaced ${{ env.GCOVR_ARGS }} in the gcovr run step with $GCOVR_ARGS (env var already set in environment). (4) Replaced ${{ env.COVERALLS_OUT }} in the curl run step with $COVERALLS_OUT (env var already set in environment). (5) Quoted $GITHUB_ENV references properly.

### Iteration 2

**Fixes applied:** script-injection

**Notes:**

Fixed the unquoted $GCOVR_ARGS shell variable expansion by eliminating the GITHUB_ENV intermediary entirely. Merged the 'Process inputs' and 'Generate code coverage report' steps into one, using a bash array (args=()) to build gcovr arguments. Each user-controlled input is placed in the step's env: block and referenced as a quoted environment variable when added to the array. gcovr is invoked as `gcovr "${args[@]}"` which is safe from word splitting and metacharacter injection. The 'Send code coverage report to Coveralls' step was updated to independently resolve the coveralls output path from its own env: variable.

