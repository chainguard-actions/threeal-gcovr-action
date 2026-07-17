<!-- markdownlint-disable -->

# Hardening Report: threeal--gcovr-action/v0.2.0

> This file was generated automatically by the hardening agent.

**Policy SHA:** `d636be7e43ef829af6e853da6b3c7566db9f72fe`

**Test Policy SHA:** `843adf9e4b8f85d0c08b27b9d0b09dd094b54702`

**Harden Agent Version:** `2`

Action **threeal--gcovr-action/v0.2.0** was hardened automatically. 17 finding(s) were identified and resolved across 3 iteration(s).

## Findings Fixed

### script-injection (severity: high)

Multiple ${{ inputs.* }} expressions are interpolated directly inside run: shell command strings in the 'Process inputs' step. For example: `if [ -n '${{ inputs.root }}' ]`, `ARGS="$ARGS --root '${{ inputs.root }}'"`, `COVERALLS_OUT='${{ inputs.coveralls-out }}'`, `ENVS="$ENVS COVERALLS_REPO_TOKEN=${{ inputs.github-token }}"`, etc. GitHub Actions substitutes these expressions into the shell script before the shell parses it, so a caller supplying a value like `'; malicious_cmd; '` can inject arbitrary shell commands. Sub-rule (a) violated.

Locations:

- `action.yml:37`

### script-injection (severity: high)

The 'Generate code coverage report using gcovr' step interpolates ${{ env.GCOVR_ENVS }}, ${{ env.GCOVR_ARGS }}, and ${{ env.COVERALLS_OUT }} directly into the run: shell command: `${{ env.GCOVR_ENVS }} gcovr ${{ env.GCOVR_ARGS }}` and `if [ -e '${{ env.COVERALLS_OUT }}' ]`. These env values were derived from untrusted inputs.* in the prior step, so injecting shell metacharacters via inputs leads to command injection here. Sub-rule (a) violated.

Locations:

- `action.yml:68`

### script-injection (severity: high)

The 'Send code coverage report to Coveralls' step interpolates ${{ env.COVERALLS_OUT }} directly into the curl command: `curl -v -F json_file=@${{ env.COVERALLS_OUT }} https://coveralls.io/api/v1/jobs`. This env value originates from the untrusted inputs.coveralls-out input, enabling script injection. Sub-rule (a) violated.

Locations:

- `action.yml:77`

### github-env-injection (severity: high)

The 'Process inputs' step writes shell variables $ENVS, $ARGS, and $COVERALLS_OUT to $GITHUB_ENV without sanitization (no `printf '%s' ... | tr -d '\n\r'` step). These variables were constructed by directly embedding ${{ inputs.root }}, ${{ inputs.gcov-executable }}, ${{ inputs.exclude }}, ${{ inputs.fail-under-line }}, ${{ inputs.coveralls-out }}, ${{ inputs.coveralls-send }}, and ${{ inputs.github-token }} values. An attacker-controlled input containing a newline can inject arbitrary key=value pairs into the runner's environment via GITHUB_ENV.

Locations:

- `action.yml:55`
- `action.yml:56`
- `action.yml:57`

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

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.github-token }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:64`

### static-inline-injection (severity: high)

shell injection: expression "${{ inputs.github-token }}" appears directly in run: block of step "Process inputs"; move to env: map

Locations:

- `action.yml:65`

## Iteration Notes

### Iteration 1

**Fixes applied:** script-injection, github-env-injection, static-inline-injection

**Notes:**

Rewrote action.yml to fix all script injection and github-env-injection findings:

1. 'Process inputs' step: Moved all ${{ inputs.* }} expressions (root, gcov-executable, exclude, fail-under-line, coveralls-out, coveralls-send, github-token) from the run: block into the step's env: block as INPUT_ROOT, INPUT_GCOV_EXECUTABLE, INPUT_EXCLUDE, INPUT_FAIL_UNDER_LINE, INPUT_COVERALLS_OUT, INPUT_COVERALLS_SEND, INPUT_GITHUB_TOKEN. The shell script now references these as plain $VAR environment variables.

2. github-env-injection: Before writing GCOVR_ENVS, GCOVR_ARGS, and COVERALLS_OUT to $GITHUB_ENV, each value is sanitized with `printf '%s' "$VAR" | tr -d '\n\r'` to strip newlines that could inject additional key=value pairs.

3. 'Generate code coverage report' step: Moved ${{ env.GCOVR_ENVS }}, ${{ env.GCOVR_ARGS }}, and ${{ env.COVERALLS_OUT }} from the run: block into the step's env: block. The shell script uses plain $GCOVR_ENVS, $GCOVR_ARGS, $COVERALLS_OUT variables.

4. 'Send code coverage report to Coveralls' step: Moved ${{ env.COVERALLS_OUT }} from the curl command into the step's env: block. The curl command now uses plain $COVERALLS_OUT variable.

### Iteration 2

**Fixes applied:** script-injection

**Notes:**

Fixed the script-injection vulnerability in the 'Generate code coverage report using gcovr' step. The original code built space-separated strings ($GCOVR_ENVS, $GCOVR_ARGS) from user inputs and used them unquoted in the shell command, enabling injection via shell metacharacters. The fix: (1) removes the GCOVR_ENVS/GCOVR_ARGS approach from GITHUB_ENV; (2) passes individual input values directly to the final step via its own env: block; (3) builds a proper bash array (gcovr_args) with each flag and value as separate, double-quoted elements; (4) sets COVERALLS_REPO_TOKEN inline as a command prefix rather than via an unquoted string. All user-controlled values are now safely quoted and passed as discrete array elements.

### Iteration 3

**Fixes applied:** unpinned-uses, missing-permissions, script-injection

**Notes:**

Fixed all three findings in .github/workflows/test.yml: (1) Pinned all 8 unpinned action references to full commit SHAs — actions/checkout@v3.3.0 → @ac593985615ec2ede58e132d2e21d2b1cbd6127c and threeal/cmake-action@v1.0.0 → @894b72b28768b1a428881a70a7ebfc0ed4cd44aa, with original tags preserved as comments. (2) Added top-level `permissions: contents: read` block. (3) Fixed script-injection in the 'Check if the previous step did fail' step by moving `steps.failed_step.outcome` into an `env:` variable (`STEP_OUTCOME`) and replacing the direct `${{ }}` expression in `run:` with a safe shell if/else construct.

