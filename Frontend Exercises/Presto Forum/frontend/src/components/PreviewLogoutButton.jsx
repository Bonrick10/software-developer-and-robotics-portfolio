import React from 'react';
import { Button } from 'react-bootstrap';

export function PreviewLogoutButton ({ setLocalToken }) {
  const logout = () => {
    setLocalToken('null');
  }
  return (
    <Button variant="danger" onClick={logout}>
      Logout
    </Button>
  );
}
