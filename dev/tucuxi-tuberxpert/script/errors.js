/* eslint-disable max-classes-per-file */

export class InvalidArgumentError extends Error {
  constructor(name, value) {
    super(`La valeur [${value}] testée pour [${name}] n'est pas une string!`);
  }
}

export class UnknownValidatorError extends Error {
  constructor(name) {
    super(`Aucun validateur trouvé pour [${name}]`);
  }
}

export class ValidationError extends Error {
  constructor(name, value) {
    super(`La valeur [${value}] n'est pas valide pour [${name}]`);
  }
}
